#include "entitymesh.h"
#include "scene.h"

EntityMesh::EntityMesh(const char* mesh, const char* texture)
{
	this->mesh = Mesh::Get(mesh);
	this->texture = Texture::Get(texture);
	this->shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");

	this->box_world = transformBoundingBox(this->model, this->mesh->box);
}

void EntityMesh::render() {
	if (shader && this->visible==TRUE)
	{
		//get the last camera that was activated
		//Camera* camera = Camera::current;
		Camera* camera = (Camera*)Scene::instance->cameras[0];
		//if the object is moving
		box_world = transformBoundingBox(this->model, mesh->box); //...
		//if(camera->testBoxInFrustum(this->box_world.center, this->box_world.halfsize)){
		if (this->render_always == TRUE || camera->testSphereInFrustum(this->position_world(), mesh->radius)) {
			//enable shader
			//shader->enable();

			//upload uniforms
			shader->setUniform("u_color", Vector4(1, 1, 1, 1));
			shader->setUniform("u_viewprojection", camera->viewprojection_matrix);
			shader->setUniform("u_texture", texture, 0);
			shader->setUniform("u_model", this->model);
			shader->setUniform("u_time", time);

			//do the draw call
			mesh->render(GL_TRIANGLES);

			//disable shader
			//shader->disable();
		}
	}

}