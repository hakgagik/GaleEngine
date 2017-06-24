#include "UI_Manager.h"
#include "../Core/IDebuggable.h"
#include "../Rendering/GameObjects/Models/Model.h"
#include "../Rendering/IRenderer.h"
#include "../Rendering/Materials/ScreenQuadMaterial.h"
#include "../Rendering/GlyphTexture.h"
#include "Texture_Manager.h"
#include <vector>
#include <sstream>
#include <glm/glm.hpp>

using namespace Core;
using namespace Managers;
using namespace Rendering;
using namespace GameObjects::Models;
using namespace Materials;
using namespace std;
using namespace glm;

UI_Manager UI_Manager::instance;

void UI_Manager::Init() {
	screenQuad = new Model(true, "Screen Quad");
	fontScale = 0.75;
	sx = fontScale / 800;
	sy = fontScale / 600;
}

UI_Manager& UI_Manager::Get() {
	return instance;
}

UI_Manager::UI_Manager() {
}

void UI_Manager::Draw(IRenderer* renderer) {
	printLine("Debug Info:", renderer);
	for (IDebuggable* debuggable : debuggableList) {
		for (string str : debuggable->GetDebugOutput()) {
			printLine(str, renderer);
		}
	}
	printLine("I had a friend once.", renderer);
	for (string str : toDisplay) {
		printLine(str, renderer);
	}
	drawFramerate(renderer);
}

void UI_Manager::Resize(int width, int height) {
	this->sx = fontScale / width;
	this->sy = fontScale / height;
}

void UI_Manager::AddToDisplay(string &str) {
	toDisplay.push_back(str);
}

void UI_Manager::EndFrame() {
	toDisplay.clear();
	x = -1 + 8 * sx;
	y = 1 - 64 * sy;
}

void UI_Manager::RegisterDebuggable(IDebuggable* debuggable) {
	debuggableList.emplace(debuggable);
}

void UI_Manager::DeregisterDebuggable(IDebuggable* debuggable) {
	debuggableList.erase(debuggable);
}

void UI_Manager::printLine(const string &text, IRenderer* renderer) {
	float ox = x;
	float oy = y;
	for (char p : text) {
		const GlyphTexture* glyphTex = Texture_Manager::Get().GetCharTexture(p);
		if (glyphTex->width != 0 && glyphTex->height != 0) {
			float x2 = ox + glyphTex->bitmap_left * sx;
			float y2 = oy + glyphTex->bitmap_top * sy;
			float w = glyphTex->width * sx;
			float h = -(glyphTex->height * sy);
			screenQuad->SetFragmentMat("Main", new ScreenQuadMaterial(vec4(x2, y2, w, h), glyphTex, vec4(0,1,0,1)));
			renderer->Render(screenQuad);
		}
		ox += glyphTex->advance_x / 64 * sx;
		oy += glyphTex->advance_y / 64 * sy;
	}
	y -= 64 * sy;
}

void UI_Manager::drawFramerate(IRenderer* renderer) {
	stringstream ss;
	ss << fixed << setprecision(2);
	ss << "Framerate: " << Framerate;
	string text = ss.str();

	float ox = 1 - 64 * 16 * sx;
	float oy = 1 - 64 * sy;

	for (char p : text) {
		const GlyphTexture* glyphTex = Texture_Manager::Get().GetCharTexture(p);
		if (glyphTex->width != 0 && glyphTex->height != 0) {
			float x2 = ox + glyphTex->bitmap_left * sx;
			float y2 = oy + glyphTex->bitmap_top * sy;
			float w = glyphTex->width * sx;
			float h = -(glyphTex->height * sy);
			screenQuad->SetFragmentMat("Main", new ScreenQuadMaterial(vec4(x2, y2, w, h), glyphTex, vec4(0, 1, 0, 1)));
			renderer->Render(screenQuad);
		}
		ox += glyphTex->advance_x / 64 * sx;
		oy += glyphTex->advance_y / 64 * sy;
	}
}

//void ForwardRenderer::RenderText(string& text, float x, float y, float sx, float sy) {
//	GLuint program = Shader_Manager::Get().GetShader("text");
//	glUseProgram(program);
//	//glBindVertexArray(1);
//	for (char p : text) {
//		const GlyphTexture* glyphTex = Texture_Manager::Get().GetCharTexture(p);
//
//		if (glyphTex->width != 0 && glyphTex->height != 0)
//		{
//			float x2 = x + glyphTex->bitmap_left * sx;
//			float y2 = -y - glyphTex->bitmap_top * sy;
//			float w = glyphTex->width * sx;
//			float h = glyphTex->height * sy;
//
//			GLfloat box[4][4] = {
//				{x2, -y2, 0, 0},
//				{x2 + w, -y2, 1, 0},
//				{x2, -y2 - h, 0, 1},
//				{x2 + w, -y2 - h, 1, 1}
//			};
//
//			useTexture(program, "tex", "null", glyphTex, 0);
//			
//			glBegin(GL_TRIANGLE_STRIP);
//			glVertex4f(box[0][0], box[0][1], box[0][2], box[0][3]);
//			glVertex4f(box[1][0], box[1][1], box[1][2], box[1][3]);
//			glVertex4f(box[2][0], box[2][1], box[2][2], box[2][3]);
//			glVertex4f(box[3][0], box[3][1], box[3][2], box[3][3]);
//			glEnd();
//			
//			//glBufferData(GL_ARRAY_BUFFER, sizeof box, box, GL_DYNAMIC_DRAW);
//			//glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
//		}
//
//		x += glyphTex->advance_x / 64 * sx;
//		y += glyphTex->advance_y / 64 * sy;
//	}
//}

//void UI_Manager::printLine()