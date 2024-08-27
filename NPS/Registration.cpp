#include <algorithm>
#include "Plate.h"
#include "Registration.h"

GLuint Registration::totalRegObjects{ 0 };
std::map<std::string, Registration::FontType> Registration::fonts;
//================================================================================================
Registration::Registration(Plate* parent) : buffer(6, true), parent(parent) {}
//================================================================================================
void Registration::Load(const std::string& filename, const std::string& tag)
{
	auto hasCarFont = (filename.find("car") != std::string::npos);

	//Car font dimensions (mm)
	const auto carFontHeight = 79;
	const auto carFontWidthWide = 50;
	const auto carFontWidthNarrow = 14;

	//Motorcycle font dimensions (mm)
	const auto motorcycleFontHeight = 64;
	const auto motorcycleFontWidthWide = 44;
	const auto motorcycleFontWidthNarrow = 10;

	auto maxDimension = Plate::GetMaxDimension();

	auto characterHeight = (hasCarFont) ? 2.0f * (carFontHeight / maxDimension.y)
		: 2.0f * (motorcycleFontHeight / maxDimension.y);

	auto wideCharacterWidth = (hasCarFont) ? 2.0f * (carFontWidthWide / maxDimension.x)
		: 2.0f * (motorcycleFontWidthWide / maxDimension.x);

	auto narrowCharacterWidth = (hasCarFont) ? 2.0f * (carFontWidthNarrow / maxDimension.x)
		: 2.0f * (motorcycleFontWidthNarrow / maxDimension.x);

	static auto totalCharSet = 0;

	for (char c = 'A'; c <= 'Z'; c++)
	{
		std::string filenameFull = filename;
		filenameFull += "/";
		filenameFull += c;
		filenameFull += ".png";

		fonts[tag].emplace(std::pair<GLchar, Glyph>(c, Glyph(filenameFull)));

		if (c == 'I')
		{
			fonts[tag][c].width = narrowCharacterWidth;
			fonts[tag][c].height = characterHeight;
		}

		else
		{
			fonts[tag][c].width = wideCharacterWidth;
			fonts[tag][c].height = characterHeight;
		}
	}

	for (char c = '0'; c <= '9'; c++)
	{
		std::string filenameFull = filename;
		filenameFull += "/";
		filenameFull += c;
		filenameFull += ".png";

		fonts[tag].emplace(std::pair<GLchar, Glyph>(c, Glyph(filenameFull)));

		if (c == '1')
		{
			fonts[tag][c].width = narrowCharacterWidth;
			fonts[tag][c].height = characterHeight;
		}

		else
		{
			fonts[tag][c].width = wideCharacterWidth;
			fonts[tag][c].height = characterHeight;
		}
	}

	totalCharSet++;
}
//================================================================================================
void Registration::Unload(const std::string& tag)
{
	auto it = fonts.find(tag);
	assert(it != fonts.end());
	fonts.erase(it);
}
//================================================================================================
Transform& Registration::GetTransform()
{
	return transform;
}
//================================================================================================
const std::string& Registration::GetString() const
{
	return string;
}
//================================================================================================
void Registration::SetFont(const std::string& tag)
{
	auto it = fonts.find(tag);
	assert(it != fonts.end());
	font = it->second;
}
//================================================================================================
void Registration::SetString(const std::string& string)
{
	this->string = string;
}
//================================================================================================
void Registration::Render(Shader& shader)
{
	shader.SendData("isText", false);
	shader.SendData("isTextured", true);

	//For two-liners, we have to calculate the top and bottom positions
	//as well as create two separate string objects to be rendered
	//These two string objects will be passed in to the private 'Render'
	//function individually as well the two starting positions for top/bottom
	//All registration positions are based on the plate's main position

	auto plateData = parent->GetPlateData();

	//if (parent->GetPlateData().isTwoLineRegistration)
	if (std::stoi(plateData["IsTwoLineRegistration"]))
	{
		std::string registrationTop;
		std::string registrationBottom;

		auto startingPositionTop = parent->GetTransform().GetPosition() + transform.GetPosition();
		auto startingPositionBottom = parent->GetTransform().GetPosition() + transform.GetPosition();

		//First check if there is a ' ' present in the registration text
		auto index = string.find(' ');

		//Needs to be in NDC
		const auto twoLineSpacing = 2.0f * ((this->twoLineSpacing + parent->GetProperties()->raisedTwoLineSpace) / parent->GetMaxDimension().x);

		//Split the text into two parts exactly where a ' ' is found
		//The top text portion will move up and the bottom segment downwards
		if (index != std::string::npos)
		{
			registrationTop = string.substr(0, index);
			registrationBottom = string.substr(index + 1, string.size() - (index + 1));

			//Move top text upward and bottom text downward
			startingPositionTop.y += font[string[0]].height *
				(0.5f * parent->GetTransform().GetScale()) + twoLineSpacing;

			startingPositionBottom.y -= font[string[0]].height *
				(0.5f * parent->GetTransform().GetScale()) + twoLineSpacing;

			Render(shader, registrationTop, startingPositionTop);
			Render(shader, registrationBottom, startingPositionBottom);
		}

		//Otherwise if no space is found then split the string after 4 characters
		else if (string.size() > 4)
		{
			registrationTop = string.substr(0, 4);
			registrationBottom = string.substr(4, string.size() - 4);

			startingPositionTop.y += ((font[string[0]].height * parent->GetTransform().GetScale()) * 0.5f)
				+ twoLineSpacing;

			startingPositionBottom.y -= ((font[string[0]].height * parent->GetTransform().GetScale()) * 0.5f)
				+ twoLineSpacing;

			Render(shader, registrationTop, startingPositionTop);
			Render(shader, registrationBottom, startingPositionBottom);
		}
	}

	//Otherwise simply treat it as a single-line registration
	else
	{
		auto position = parent->GetTransform().GetPosition() + transform.GetPosition();
		Render(shader, string, position);
	}

	//if (string.size() > parent->GetPlateData().maxAllowedCharacters)
	if (string.size() > std::stoi(plateData["MaxAllowedCharacters"]))
	{
		parent->IsLegal(false);
	}
}
//================================================================================================
void Registration::Render(Shader& shader, const std::string& registration, glm::vec3& position)
{
	GLfloat totalCharacterWidth = 0.0f;
	auto& parentPosition = parent->GetTransform().GetPosition();

	const auto spacebarSpacing = 2.0f * (this->spacebarSpacing / parent->GetMaxDimension().x);
	const auto characterSpacing = 2.0f * (this->characterSpacing / parent->GetMaxDimension().x);

	//Loop through all letters in registration and calculate the total width
	//We later halve this total width so that we can place the text in the middle
	for (auto& character : registration)
	{
		if (character != ' ')
		{
			Glyph glyph = font[character];

			totalCharacterWidth += glyph.width * transform.GetScale();
		}

		else
		{
			totalCharacterWidth += spacebarSpacing * transform.GetScale();
		}
	}

	//Only if there are multiple letters in the registration then we add spacing
	//There is always one less spacing than there are letters in the text and if
	//a spacebar was pressed this counts as its own spacing meaning we have two less
	if (registration.size() > 1)
	{
		if (registration.find(' ') == std::string::npos)
		{
			totalCharacterWidth += (registration.size() - 1) *
				(characterSpacing * transform.GetScale());
		}

		else
		{
			totalCharacterWidth += ((registration.size() - 2) *
				(characterSpacing * transform.GetScale()));
		}
	}

	position.x -= totalCharacterWidth * 0.5f;

	auto reg = registration;

	//Loop through all letters in the registration text and position/render 
	//them accordingly. We do not have a texture for space bar spacing so 
	//we simply advance the position to avoid crashes when indexing the map
	for (auto& character : reg)
	{
		if (character == ' ')
		{
			position.x += spacebarSpacing * transform.GetScale();
			position.x -= characterSpacing;
		}

		else
		{
			Glyph glyph = font[character];

			position.x += (glyph.width * transform.GetScale()) * 0.5f;

			Transform transform;
			transform.SetPosition(position);
			shader.SendData("model", transform.GetMatrix());

			auto halfDimension = glm::vec2(glyph.width, glyph.height) * 0.5f * transform.GetScale();

			GLfloat vertices[] = { -halfDimension.x,  halfDimension.y, 0.0f,
								halfDimension.x,  halfDimension.y, 0.0f,
								halfDimension.x, -halfDimension.y, 0.0f,
							   -halfDimension.x, -halfDimension.y, 0.0f };

			GLfloat colors[] = { 1.0f, 1.0f, 1.0f, 1.0f,
								 1.0f, 1.0f, 1.0f, 1.0f,
								 1.0f, 1.0f, 1.0f, 1.0f,
								 1.0f, 1.0f, 1.0f, 1.0f };

			GLfloat UVs[] = { 0.0f, 1.0f,
							  1.0f, 1.0f,
							  1.0f, 0.0f,
							  0.0f, 0.0f };

			GLuint indices[] = { 0, 1, 3, 3, 1, 2 };

			buffer.FillVBO(Buffer::VBO::VertexBuffer, vertices, sizeof(vertices), Buffer::Fill::Ongoing);
			buffer.FillVBO(Buffer::VBO::ColorBuffer, colors, sizeof(colors), Buffer::Fill::Ongoing);
			buffer.FillVBO(Buffer::VBO::TextureBuffer, UVs, sizeof(UVs), Buffer::Fill::Ongoing);
			buffer.FillEBO(indices, sizeof(indices), Buffer::Fill::Ongoing);
			buffer.LinkEBO();

			buffer.LinkVBO(shader.GetAttributeID("vertexIn"),
				Buffer::VBO::VertexBuffer, Buffer::ComponentSize::XYZ, Buffer::DataType::FloatData);
			buffer.LinkVBO(shader.GetAttributeID("colorIn"),
				Buffer::VBO::ColorBuffer, Buffer::ComponentSize::RGBA, Buffer::DataType::FloatData);
			buffer.LinkVBO(shader.GetAttributeID("textureIn"),
				Buffer::VBO::TextureBuffer, Buffer::ComponentSize::UV, Buffer::DataType::FloatData);

			glyph.texture.Bind();
			buffer.Render(Buffer::RenderMode::Triangles);
			glyph.texture.Unbind();

			position.x += ((glyph.width * transform.GetScale()) * 0.5f) + (characterSpacing * transform.GetScale());
		}
	}
}