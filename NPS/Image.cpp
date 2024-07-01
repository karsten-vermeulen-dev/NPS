#include "Image.h" 
#include "Input.h"

//================================================================================================
Image::Image(const std::string& filename) : buffer(6, true), texture(filename)
{
	GLfloat vertices[] = { -1.0f,  1.0f, 0.0f,
							1.0f,  1.0f, 0.0f,
							1.0f, -1.0f, 0.0f,
						   -1.0f, -1.0f, 0.0f };

	GLfloat colors[] = { 1.0f, 1.0f, 1.0f, 1.0f,
						 1.0f, 1.0f, 1.0f, 1.0f,
						 1.0f, 1.0f, 1.0f, 1.0f,
						 1.0f, 1.0f, 1.0f, 1.0f };

	GLfloat UVs[] = { 0.0f, 1.0f,
					  1.0f, 1.0f,
					  1.0f, 0.0f,
					  0.0f, 0.0f };

	GLfloat normals[] = { 0.0f, 0.0f, 1.0f,
						  0.0f, 0.0f, 1.0f,
						  0.0f, 0.0f, 1.0f,
						  0.0f, 0.0f, 1.0f };

	GLuint indices[] = { 0, 1, 3,
						 3, 1, 2 };

	buffer.LinkEBO();

	buffer.FillEBO(indices, sizeof(indices));
	buffer.FillVBO(Buffer::VBO::VertexBuffer,
		vertices, sizeof(vertices), Buffer::Fill::Ongoing);
	buffer.FillVBO(Buffer::VBO::ColorBuffer,
		colors, sizeof(colors), Buffer::Fill::Ongoing);
	buffer.FillVBO(Buffer::VBO::TextureBuffer,
		UVs, sizeof(UVs), Buffer::Fill::Ongoing);
	buffer.FillVBO(Buffer::VBO::NormalBuffer,
		normals, sizeof(normals), Buffer::Fill::Ongoing);
}
//================================================================================================
Transform& Image::GetTransform()
{
	return transform;
}
//================================================================================================
void Image::SetDimension(GLfloat width, GLfloat height)
{
	auto halfWidth = width * 0.5f;
	auto halfHeight = height * 0.5f;

	GLfloat vertices[] = { -halfWidth,  halfHeight, 0.0f,
							halfWidth,  halfHeight, 0.0f,
							halfWidth, -halfHeight, 0.0f,
						   -halfWidth, -halfHeight, 0.0f };

	buffer.FillVBO(Buffer::VBO::VertexBuffer,
		vertices, sizeof(vertices), Buffer::Fill::Ongoing);
}
//================================================================================================
void Image::Update()
{
	if (Input::Instance()->IsLeftButtonClicked())
	{
		static auto eulerAngles = glm::vec3(0.0f);
		eulerAngles.x += Input::Instance()->GetMouseMotion().y;
		eulerAngles.y += Input::Instance()->GetMouseMotion().x;
		transform.SetRotation(eulerAngles);
	}

	auto position = transform.GetPosition();
	position.z += Input::Instance()->GetMouseWheel().y;
	transform.SetPosition(position);
}
//================================================================================================
void Image::Render(Shader& shader, bool isLit)
{
	shader.SendData("model", transform.GetMatrix());
	shader.SendData("isTextured", true);

	buffer.LinkVBO(shader.GetAttributeID("vertexIn"),
		Buffer::VBO::VertexBuffer, Buffer::ComponentSize::XYZ, Buffer::DataType::FloatData);
	buffer.LinkVBO(shader.GetAttributeID("colorIn"),
		Buffer::VBO::ColorBuffer, Buffer::ComponentSize::RGBA, Buffer::DataType::FloatData);
	buffer.LinkVBO(shader.GetAttributeID("textureIn"),
		Buffer::VBO::TextureBuffer, Buffer::ComponentSize::UV, Buffer::DataType::FloatData);
	
	if (isLit)
	{
		buffer.LinkVBO(shader.GetAttributeID("normalIn"),
			Buffer::VBO::NormalBuffer, Buffer::ComponentSize::XYZ, Buffer::DataType::FloatData);

		normalMatrix = glm::inverse(transform.GetMatrix());
		shader.SendData("normal", normalMatrix, true);
		
		shader.SendData("light.ambient", glm::vec3(1.0f));
		shader.SendData("light.diffuse", glm::vec3(1.0f));
		shader.SendData("light.specular", glm::vec3(1.0f));
		shader.SendData("light.position", glm::vec3(0.0f, 0.0f, 1.0f));

		shader.SendData("cameraPosition", glm::vec3(0.0f, 0.0f, 0.0f));

		shader.SendData("material.ambient", ambient);
		shader.SendData("material.diffuse", diffuse);
		shader.SendData("material.specular", specular);
		shader.SendData("material.shininess", shininess);
	}

	texture.Bind();
	buffer.Render(Buffer::RenderMode::Triangles);
	texture.Unbind();
}