#pragma once


#include "physx/PxPhysicsAPI.h"
using namespace physx;

#include "box2d/box2d.h"

#include "MathExtras.h"
#include "FontCreation.h"



	struct Textbox {
	public:
		Textbox(char* _textToLoad, FontCreation::SetupFace* _fontManager, Vec2 _textRect, Vec2 _textPos, float _fontSize, HWND _hWnd)
			: textToLoad(_textToLoad), fontManager(_fontManager), textRect(_textRect), textPos(_textPos), fontSize(_fontSize), hWnd(_hWnd), rows(0)
		{}

		Textbox()
			: rows(0)
		{}

		char* textToLoad;

		FontCreation::SetupFace* fontManager;


		void LoadText();

	public:
		Vec2 textRect;
		Vec2 textPos;

		struct TextGlyph {
			TextGlyph(Vec2 _pos, Vec2 _rect, FT_UInt _index, FT_Glyph _glyph)
			{
				pos = _pos;
				rect = _rect;
				index = _index; 
				glyph = _glyph;
			}
			TextGlyph()
			{
				pos = Vec2(0,0);
				rect = Vec2(0, 0);
				index = 0;
				glyph = NULL;
			}

			Vec2 pos;
			Vec2 rect;
			FT_UInt index;
			FT_Glyph glyph;
		};

		std::vector<TextGlyph> glyphs;


		float fontSize;

		int rows = 0;

		HWND hWnd;
	};

	
	struct Button {
	public:

		Button()
		{}

		void CreateActor(PxPhysics* physics, PxScene* scene) {


			//PxTriangleMeshDesc meshDesc;
			//meshDesc.points.count = vertices.size();
			//meshDesc.points.stride = sizeof(PxVec3);
			//meshDesc.points.data = vertices.data();

			//meshDesc.triangles.count = indices.size() / 3;
			//meshDesc.triangles.stride = 3 * sizeof(PxU32);
			//meshDesc.triangles.data = indices.data();


			//PxDefaultMemoryOutputStream writeBuffer;
			//PxTriangleMeshCookingResult::Enum result;
			//bool status = cooking->cookTriangleMesh(meshDesc, writeBuffer, &result);
			//if (!status) {
			//	// Handle error
			//}
			//PxDefaultMemoryInputData readBuffer(writeBuffer.getData(), writeBuffer.getSize());
			//PxTriangleMesh* triangleMesh = physics->createTriangleMesh(readBuffer);


			//PxMaterial* material = physics->createMaterial(0.5f, 0.5f, 0.6f); // static friction, dynamic friction, restitution

			//PxShape* shape = physics->createShape(PxTriangleMeshGeometry(triangleMesh), *material);


			//PxRigidDynamic* actor = physics->createRigidDynamic(PxTransform(PxVec3(0, 0, 0)));
			//actor->attachShape(*shape);
			//// Update the mass and inertia properties based on the shape
			//PxRigidBodyExt::updateMassAndInertia(*actor, 10.0f); // density
			//// Add the actor to the scene
			//scene->addActor(*actor);


			//// Add the actor to the scene
			//scene->addActor(*actor);
		}

		void CreateFixture(b2Body* body) {
			std::vector<b2Vec2> vertices;

			vertices.push_back(b2Vec2(-1.0f, 1.0f));
			vertices.push_back(b2Vec2(1.0f, 1.0f));
			vertices.push_back(b2Vec2(1.0f, -1.0f));
			vertices.push_back(b2Vec2(-1.0f, -1.0f));
			b2PolygonShape shape;
			shape.Set(&vertices[0], static_cast<int32>(vertices.size()));

			b2FixtureDef fixtureDef;
			fixtureDef.shape = &shape;
			//fixtureDef.density = 1.0f;
			//fixtureDef.friction = 0.3f;
			//fixtureDef.restitution = 0.5f;

			fixture = body->CreateFixture(&fixtureDef);
		}
		
	public:

		std::vector<PxVec3> vertices;
		std::vector<PxU32> indices;

		b2Fixture* fixture;
	};