#include <algorithm>
#include <assert.h>
#include "Plate.h"
#include "Screen.h"
#include "Text.h"
#include "Utility.h"

FT_Library Text::freetypeObject{ nullptr };
std::map<std::string, Text::FontType> Text::fonts;

//================================================================================================
bool Text::Initialize()
{
	if (FT_Init_FreeType(&freetypeObject))
	{
		Utility::Log(Utility::Destination::WindowsMessageBox,
			"Error initializing the FreeType font system.", Utility::Severity::Failure);
		return false;
	}

	return true;
}
//================================================================================================
void Text::Shutdown()
{
	FT_Done_FreeType(freetypeObject);
}
//================================================================================================
bool Text::Load(const std::string& filename, GLfloat fontHeight, const std::string& tag)
{
	assert(fonts.find(tag) == fonts.end());
	FT_Face freetypeFace = nullptr;

	if (FT_New_Face(freetypeObject, filename.c_str(), 0, &freetypeFace))
	{
		Utility::Log(Utility::Destination::WindowsMessageBox,
			"Error loading font file \"" + filename + "\"\n\n"
			"Possible causes could be a corrupt or missing file. Another reason could be "
			"that the filename and/or path are incorrectly spelt.", Utility::Severity::Failure);

		return false;
	}

	auto maxPlateDimension = Plate::GetMaxDimension();
	auto viewport = Screen::Instance()->GetViewport();
	auto aspectRatio = viewport.w / static_cast<float>(viewport.h);

	//This is the pixel dimension (mm -> px)
	auto fontSize = std::ceil(viewport.h * (fontHeight / maxPlateDimension.y));

	FT_Set_Pixel_Sizes(freetypeFace, (FT_UInt)fontSize, (FT_UInt)fontSize);

	//This is needed so that we don't follow the standard 4-byte alignment
	//because the freetype glyph textures only use 1 byte of color data
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	FontType font;

	//Generate a glyph for all 128 ASCII characters
	for (int i = 0; i < 128; i++)
	{
		if (FT_Load_Char(freetypeFace, i, FT_LOAD_RENDER))
		{
			Utility::Log(Utility::Destination::WindowsMessageBox,
				"Error creating glyph", Utility::Severity::Warning);
			continue;
		}

		GLuint textureID;
		glGenTextures(1, &textureID);
		glBindTexture(GL_TEXTURE_2D, textureID);

		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glTexImage2D(GL_TEXTURE_2D,
			0,
			GL_RED,
			freetypeFace->glyph->bitmap.width,
			freetypeFace->glyph->bitmap.rows,
			0,
			GL_RED,
			GL_UNSIGNED_BYTE,
			freetypeFace->glyph->bitmap.buffer);

		glBindTexture(GL_TEXTURE_2D, 0);

		Glyph glyph = { textureID,
						static_cast<GLint>(freetypeFace->glyph->bitmap.width),
						static_cast<GLint>(freetypeFace->glyph->bitmap.rows),
						static_cast<GLint>(freetypeFace->glyph->bitmap_left),
						static_cast<GLint>(freetypeFace->glyph->bitmap_top),
						static_cast<GLint>(freetypeFace->glyph->advance.x),
						fontHeight };

		font[i] = glyph;
	}

	FT_Done_Face(freetypeFace);
	fonts[tag] = font;
	return true;
}
//================================================================================================
void Text::Unload(const std::string& tag)
{
	auto it = fonts.find(tag);
	//assert(it != fonts.end());
	
	if (it != fonts.end())
	{
		for (const auto& glyph : it->second)
		{
			glDeleteTextures(1, &glyph.second.ID);
		}

		fonts.erase(it);
	}
}
//================================================================================================
Text::Text(Plate* parent, const std::string& tag) : buffer(6, true), parent(parent)
{
	SetFont(tag);
	buffer.LinkEBO();
}
//================================================================================================
Transform& Text::GetTransform()
{
	return transform;
}
//================================================================================================
GLfloat Text::GetHeight() const
{
	return height;
}
//================================================================================================
GLfloat Text::GetMaxWidth() const
{
	//We calculate the max width of the text right here, because we 
	//don't want to make an unnecessary Render() call to calculate it
	auto viewport = Screen::Instance()->GetViewport();

	FontType ft = font;
	auto str = string;
	auto maxWidth = 0.0f;

	for (const auto& character : str)
	{
		auto glyph = ft[character];
		auto glyphAdvance = static_cast<float>(2.0f * ((glyph.advance / 64.0f) / viewport.w));
		maxWidth += glyphAdvance;
	}

	return maxWidth;
}
//================================================================================================
void Text::SetFont(const std::string& tag)
{
	auto it = fonts.find(tag);
	assert(it != fonts.end());
	font = it->second;
}
//================================================================================================
void Text::SetString(const std::string& string)
{
	this->string = string;
}
//================================================================================================
void Text::SetColor(const glm::vec4& color)
{
	this->color = color;
}
//================================================================================================
void Text::Render(Shader& shader)
{
	shader.SendData("isText", true);
	shader.SendData("isTextured", false);
	shader.SendData("model", transform.GetMatrix());

	auto startX = 0.0f;
	auto str = string;
	auto viewport = Screen::Instance()->GetViewport();

	for (const auto& character : str)
	{
		Glyph glyph = font[character];

		auto glyphWidth = 2.0f * (glyph.width / (float)viewport.w);
		auto glyphHeight = 2.0f * (glyph.height / (float)viewport.h);

		auto glyphBearingX = 2.0f * (glyph.bearingX / (float)viewport.w);
		auto glyphBearingY = 2.0f * (glyph.bearingY / (float)viewport.h);

		GLfloat vertices[] = { startX + glyphBearingX, glyphBearingY, 0.0f,
							   startX + glyphBearingX + glyphWidth, glyphBearingY, 0.0f,
							   startX + glyphBearingX + glyphWidth, glyphBearingY - glyphHeight, 0.0f,
							   startX + glyphBearingX, glyphBearingY - glyphHeight, 0.0f };

		GLfloat colors[] = { color.r, color.g, color.b, color.a,
			color.r, color.g, color.b, color.a,
			color.r, color.g, color.b, color.a,
			color.r, color.g, color.b, color.a };

		GLfloat UVs[] = { 0.0f, 0.0f,
			1.0f, 0.0f,
			1.0f, 1.0f,
			0.0f, 1.0f };

		GLuint indices[] = { 0, 1, 3,
			3, 1, 2 };

		buffer.FillEBO(indices, sizeof(indices));
		buffer.FillVBO(Buffer::VBO::VertexBuffer, vertices, sizeof(vertices), Buffer::Fill::Ongoing);
		buffer.FillVBO(Buffer::VBO::ColorBuffer, colors, sizeof(colors), Buffer::Fill::Ongoing);
		buffer.FillVBO(Buffer::VBO::TextureBuffer, UVs, sizeof(UVs), Buffer::Fill::Ongoing);

		buffer.LinkVBO(shader.GetAttributeID("vertexIn"),
			Buffer::VBO::VertexBuffer, Buffer::ComponentSize::XYZ, Buffer::DataType::FloatData);
		buffer.LinkVBO(shader.GetAttributeID("colorIn"),
			Buffer::VBO::ColorBuffer, Buffer::ComponentSize::RGBA, Buffer::DataType::FloatData);
		buffer.LinkVBO(shader.GetAttributeID("textureIn"),
			Buffer::VBO::TextureBuffer, Buffer::ComponentSize::UV, Buffer::DataType::FloatData);

		glBindTexture(GL_TEXTURE_2D, glyph.ID);

		buffer.Render(Buffer::RenderMode::Triangles);

		glBindTexture(GL_TEXTURE_2D, 0);

		//We have to divide by 64 because the value is a product of 64
		auto glyphAdvance = static_cast<float>(2.0f * ((glyph.advance / 64.0f) / viewport.w));
		startX += glyphAdvance;

		//This is the NDC dimension (mm -> ndc)
		height = 2.0f * (glyph.heightMillimeter / Plate::GetMaxDimension().y);
	}

	shader.SendData("isText", false);
}