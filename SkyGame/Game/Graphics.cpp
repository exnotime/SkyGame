#include "Graphics.h"

GraphicsEngine::GraphicsEngine() {

}

GraphicsEngine::~GraphicsEngine() {

}

void GraphicsEngine::Init() {

}

void GraphicsEngine::Render() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}