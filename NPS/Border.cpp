#include <algorithm>
#include <vector>
#include "Border.h"
#include "Plate.h"
#include "Utility.h"
#include "Screen.h"

//================================================================================================
Border::Border(Plate* parent) : parent(parent)
{
	//The border can be max 6mm in size
	//This was an older conversion (2px for every mm)
	//TODO - Do we need this, and if so, place this const with the others
	const auto maxBorderSize = 6 * 2;

	//Each side line consists of 2 vertices and the bottom line has 
	//two separate lines to accomodate for dealer name (10 vertices)
	//Each corner has x vertices joining to make lines ((x - 1) * 2)
	//Multiplying by 'maxBorderSize' allows us to repeat creating the 
	//border for each mm/px of width and all of this is repeated three
	//times to make sure that for every border line there are two extra 
	//rendered else the lines appear dark grey instead of pure black

	const auto maxBufferSize = (3 * maxBorderSize) *
		((4 * ((parent->maxCornerPoints - 1) * 2)) + 10);

	buffer = std::make_unique<Buffer>(maxBufferSize, false);

	//Calculate the ideal pixel thickness for the border line
	//We base the pixel amount off of a 2mm thickness for each line
	const auto mm = 2.0f;
	auto viewport = Screen::Instance()->GetViewport();
	auto mmNDC = 2.0f * (mm / parent->GetMaxDimension().x);
	lineThickness = std::floor(viewport.w * (mmNDC / 2.0f));
}
//================================================================================================
void Border::Render(Shader& shader)
{
	//TODO - Bring this back later
	//if (m_isDirty)
	{
		FillBuffers();
		isDirty = false;
	}

	buffer->LinkVBO(shader.GetAttributeID("vertexIn"),
		Buffer::VBO::VertexBuffer, Buffer::ComponentSize::XYZ, Buffer::DataType::FloatData);
	buffer->LinkVBO(shader.GetAttributeID("colorIn"),
		Buffer::VBO::ColorBuffer, Buffer::ComponentSize::RGBA, Buffer::DataType::FloatData);

	Buffer::SetLineWidth(lineThickness);
	buffer->Render(Buffer::RenderMode::Lines);
}
//================================================================================================
void Border::FillBuffers()
{
	std::vector<GLfloat> colorsBorder;
	std::vector<GLfloat> verticesBorder;

	auto sideBadgeMargin = 0.0f;
	const auto maxPlateDimension = parent->GetMaxDimension();

	if (parent->GetProperties().isSideBadgeVisible)
	{
		sideBadgeMargin = 2.0f * (parent->sideBadgeMargin / maxPlateDimension.x);
	}

	const auto sideDimension = 2.0f * (parent->sideDimension / maxPlateDimension);
	const auto halfDimension = glm::vec2(parent->GetPlateData().dimensionNDC) * 0.5f;
	const auto middleDimension = halfDimension - sideDimension;

	const auto marginSize = parent->GetProperties().marginSize;
	const auto delta = 2.0f * glm::vec2(1.0f / maxPlateDimension.x, 1.0f / maxPlateDimension.y);
	const auto marginNDC = glm::vec2(2.0f * (marginSize / maxPlateDimension.x), 2.0f * (marginSize / maxPlateDimension.y));

	//Loop through each mm/px of width of the border to repeat creation of the border
	//With each iteration we move one layer deeper into the border thereby shrinking it
	for (auto j = 0; j < parent->GetProperties().borderSize; j++)
	{
		//Make sure to add color for all side lines and each corner
		for (GLuint k = 0; k < (4 * ((parent->maxCornerPoints - 1) * 2)) + 10; k++)
		{
			colorsBorder.push_back(0.0f);
			colorsBorder.push_back(0.0f);
			colorsBorder.push_back(0.0f);
			colorsBorder.push_back(1.0f);
		}

		//====================================================================================
		//TOP LINE
		//====================================================================================

		verticesBorder.push_back(-middleDimension.x + marginNDC.x + sideBadgeMargin);
		verticesBorder.push_back(halfDimension.y - (j * delta.y) - marginNDC.y);
		verticesBorder.push_back(0.0f);

		verticesBorder.push_back(middleDimension.x - marginNDC.x);
		verticesBorder.push_back(halfDimension.y - (j * delta.y) - marginNDC.y);
		verticesBorder.push_back(0.0f);

		//=======================================================================================
		//BOTTOM LINE
		//=======================================================================================

		//Text padding for dealer, postcode and BSAU text
		const auto textPadding = 2.0f * (parent->textPadding / maxPlateDimension.x);

		//Left line point #1
		verticesBorder.push_back(-middleDimension.x + marginNDC.x + sideBadgeMargin);
		verticesBorder.push_back(-halfDimension.y + (j * delta.y) + marginNDC.y);
		verticesBorder.push_back(0.0f);

		//If the dealer name is visible then the bottom line has to leave a gap so we move 
		//the end of bottom line #1 to the left and the start of bottom line #2 to the right
		//We divide EU logo margin and dealer text size in half because dealer text is centered
		if (parent->GetProperties().isDealerVisible)
		{
			auto dealerPostcodeWidth = 0.0f;

			if (parent->GetProperties().isDealerAbovePostcode)
			{
				dealerPostcodeWidth = std::max(parent->GetDealerText()->GetMaxWidth(),
					parent->GetDealerText()->GetMaxWidth());
			}

			else
			{
				dealerPostcodeWidth = parent->GetDealerText()->GetMaxWidth()
					+ parent->GetPostcodeText()->GetMaxWidth();
			}

			//Left line point #2
			verticesBorder.push_back(-0.5f * dealerPostcodeWidth - textPadding + (0.5f * sideBadgeMargin));
			verticesBorder.push_back(-halfDimension.y + (j * delta.y) + marginNDC.y);
			verticesBorder.push_back(0.0f);

			//Right line point #1
			verticesBorder.push_back(0.5f * dealerPostcodeWidth + textPadding + (0.5f * sideBadgeMargin));
			verticesBorder.push_back(-halfDimension.y + (j * delta.y) + marginNDC.y);
			verticesBorder.push_back(0.0f);
		}

		else
		{
			//Left line point #2
			verticesBorder.push_back(0.0f);
			verticesBorder.push_back(-halfDimension.y + (j * delta.y) + marginNDC.y);
			verticesBorder.push_back(0.0f);

			//Right line point #1
			verticesBorder.push_back(0.0f);
			verticesBorder.push_back(-halfDimension.y + (j * delta.y) + marginNDC.y);
			verticesBorder.push_back(0.0f);
		}

		if (parent->GetProperties().isBSAUVisible && parent->GetProperties().isBSAUOnBorder)
		{
			//Right line point #2
			verticesBorder.push_back(middleDimension.x - parent->GetBSAUText()->GetMaxWidth() - (2.0f * textPadding) - marginNDC.x);
		}

		else
		{
			//Right line point #2
			verticesBorder.push_back(middleDimension.x - marginNDC.x);
		}

		verticesBorder.push_back(-halfDimension.y + (j * delta.y) + marginNDC.y);
		verticesBorder.push_back(0.0f);

		//====================================================================================

		//LEFT LINE
		//Add on margin space for EU logo if that has been enabled
		verticesBorder.push_back(-halfDimension.x + (j * delta.x) + marginNDC.x + sideBadgeMargin);
		verticesBorder.push_back(-middleDimension.y + marginNDC.y);
		verticesBorder.push_back(0.0f);

		verticesBorder.push_back(-halfDimension.x + (j * delta.x) + marginNDC.x + sideBadgeMargin);
		verticesBorder.push_back(middleDimension.y - marginNDC.y);
		verticesBorder.push_back(0.0f);

		//====================================================================================

		//RIGHT LINE
		verticesBorder.push_back(halfDimension.x - (j * delta.x) - marginNDC.x);
		verticesBorder.push_back(-middleDimension.y + marginNDC.y);
		verticesBorder.push_back(0.0f);

		verticesBorder.push_back(halfDimension.x - (j * delta.x) - marginNDC.x);
		verticesBorder.push_back(middleDimension.y - marginNDC.y);
		verticesBorder.push_back(0.0f);

		//====================================================================================

		//TOP RIGHT CORNER
		GLfloat angle = 0.0f;
		GLfloat angleIncrements = 90.0f / (parent->maxCornerPoints - 1);

		for (GLuint k = 0; k < parent->maxCornerPoints - 1; k++)
		{
			verticesBorder.push_back(cos(glm::radians(angle)) *
				(sideDimension.x - (j * delta.x)) + middleDimension.x - marginNDC.x);
			verticesBorder.push_back(sin(glm::radians(angle)) *
				(sideDimension.y - (j * delta.y)) + middleDimension.y - marginNDC.y);

			verticesBorder.push_back(0.0f);

			angle += angleIncrements;

			verticesBorder.push_back(cos(glm::radians(angle)) *
				(sideDimension.x - (j * delta.x)) + middleDimension.x - marginNDC.x);
			verticesBorder.push_back(sin(glm::radians(angle)) *
				(sideDimension.y - (j * delta.y)) + middleDimension.y - marginNDC.y);

			verticesBorder.push_back(0.0f);
		}

		//====================================================================================

		//TOP LEFT CORNER
		//Add on margin space for EU logo if that has been enabled
		angle = 90.0f;

		for (GLuint k = 0; k < parent->maxCornerPoints - 1; k++)
		{
			verticesBorder.push_back(cos(glm::radians(angle)) *
				(sideDimension.x - (j * delta.x)) - middleDimension.x + marginNDC.x + sideBadgeMargin);
			verticesBorder.push_back(sin(glm::radians(angle)) *
				(sideDimension.y - (j * delta.y)) + middleDimension.y - marginNDC.y);

			verticesBorder.push_back(0.0f);

			angle += angleIncrements;

			verticesBorder.push_back(cos(glm::radians(angle)) *
				(sideDimension.x - (j * delta.x)) - middleDimension.x + marginNDC.x + sideBadgeMargin);
			verticesBorder.push_back(sin(glm::radians(angle)) *
				(sideDimension.y - (j * delta.y)) + middleDimension.y - marginNDC.y);

			verticesBorder.push_back(0.0f);
		}

		//====================================================================================

		//BOTTOM LEFT CORNER
		//Add on margin space for EU logo if that has been enabled
		angle = 180.0f;

		for (GLuint k = 0; k < parent->maxCornerPoints - 1; k++)
		{
			verticesBorder.push_back(cos(glm::radians(angle)) *
				(sideDimension.x - (j * delta.x)) - middleDimension.x + marginNDC.x + sideBadgeMargin);
			verticesBorder.push_back(sin(glm::radians(angle)) *
				(sideDimension.y - (j * delta.y)) - middleDimension.y + marginNDC.y);

			verticesBorder.push_back(0.0f);

			angle += angleIncrements;

			verticesBorder.push_back(cos(glm::radians(angle)) *
				(sideDimension.x - (j * delta.x)) - middleDimension.x + marginNDC.x + sideBadgeMargin);
			verticesBorder.push_back(sin(glm::radians(angle)) *
				(sideDimension.y - (j * delta.y)) - middleDimension.y + marginNDC.y);

			verticesBorder.push_back(0.0f);
		}

		//====================================================================================

		//BOTTOM RIGHT CORNER
		angle = 270.0f;

		for (GLuint k = 0; k < parent->maxCornerPoints - 1; k++)
		{
			verticesBorder.push_back(cos(glm::radians(angle)) *
				(sideDimension.x - (j * delta.x)) + middleDimension.x - marginNDC.x);
			verticesBorder.push_back(sin(glm::radians(angle)) *
				(sideDimension.y - (j * delta.y)) - middleDimension.y + marginNDC.y);

			verticesBorder.push_back(0.0f);

			angle += angleIncrements;

			verticesBorder.push_back(cos(glm::radians(angle)) *
				(sideDimension.x - (j * delta.x)) + middleDimension.x - marginNDC.x);
			verticesBorder.push_back(sin(glm::radians(angle)) *
				(sideDimension.y - (j * delta.y)) - middleDimension.y + marginNDC.y);

			verticesBorder.push_back(0.0f);
		}
	}

	assert(!verticesBorder.empty());

	buffer->FillVBO(Buffer::VBO::VertexBuffer,
		&verticesBorder[0], sizeof(GLfloat) * verticesBorder.size(), Buffer::Fill::Ongoing);
	buffer->FillVBO(Buffer::VBO::ColorBuffer,
		&colorsBorder[0], sizeof(GLfloat) * colorsBorder.size(), Buffer::Fill::Ongoing);
}