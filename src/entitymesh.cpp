#include "entitymesh.h"

EntityMesh::EntityMesh(Mesh* mesh, Texture* texture, Shader* shader)
{
	this->mesh = mesh;
	this->texture = texture;
	this->shader = shader;

	this->box_world = transformBoundingBox(this->model, mesh->box);
}

void EntityMesh::render() {
	if (shader)
	{
		//get the last camera that was activated
		//Camera* camera = Camera::current;
		Camera* camera = (Camera*)Scene::instance->entities[0];
		//if the object is moving
		box_world = transformBoundingBox(this->model, mesh->box);
		if(camera->testBoxInFrustum(this->box_world.center, this->box_world.halfsize)){
		//if (camera->testSphereInFrustum(this->position_world(), mesh->radius)) {
			//enable shader
			//shader->enable();

			//upload uniforms
			shader->setUniform("u_color", Vector4(1, 1, 1, 1));
			shader->setUniform("u_viewprojection", camera->viewprojection_matrix);
			shader->setUniform("u_texture", texture, 0);
			shader->setUniform("u_model", this->model);
			shader->setUniform("u_time", time);

			//do the draw call
			//mesh->render( GL_TRIANGLES );
			mesh->render(GL_TRIANGLES);

			//disable shader
			//shader->disable();
			//... shader enable i disable fuera
		}
	}

}