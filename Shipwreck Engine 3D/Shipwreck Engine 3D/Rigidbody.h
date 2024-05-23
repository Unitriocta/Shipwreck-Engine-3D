#pragma once

#include "testing.h"

class Rigidbody
{
public:
    Rigidbody(Physics* _physics, bool dynamic)
    {
        physicsManager = _physics;
        isDynamic = dynamic;
    }

    Rigidbody()
    {
        physicsManager = nullptr;
        isDynamic = false;
    }

	void NewRB() {
        
        rbStatic = nullptr;
        rbDynamic = nullptr;


        // disable mesh cleaning - perform mesh validation on development configurations
        params.meshPreprocessParams |= PxMeshPreprocessingFlag::eDISABLE_CLEAN_MESH;
        // disable edge precompute, edges are set for each triangle, slows contact generation
        params.meshPreprocessParams |= PxMeshPreprocessingFlag::eDISABLE_ACTIVE_EDGES_PRECOMPUTE;

        shapeFlags = PxShapeFlag::eSCENE_QUERY_SHAPE | PxShapeFlag::eSIMULATION_SHAPE | PxShapeFlag::eVISUALIZATION;
		
        if (isDynamic) {    

            PxReal angle = 0/*40 / 57.2958f*/;
            PxVec3 rot(0.0f, 0.0f, 0.0f);
            rbDynamic = physicsManager->physics->createRigidDynamic(PxTransform(PxVec3(0.0f, 16.0f, 5.0f), PxQuat(angle, rot)));
            rbDynamic->setLinearDamping(0.2f);
            rbDynamic->setAngularDamping(0.1f);
            
            rbDynamic->setRigidBodyFlag(PxRigidBodyFlag::eENABLE_GYROSCOPIC_FORCES, true);
            rbDynamic->setRigidBodyFlag(PxRigidBodyFlag::eENABLE_SPECULATIVE_CCD, true);

            /*PxTriangleMeshGeometry geom(triangleMesh);
            geom.scale = PxVec3(1.0f, 1.0f, 1.0f);*/



           PxShape* triMeshShape = PxRigidActorExt::createExclusiveShape(*rbDynamic, PxSphereGeometry(1), *physicsManager->mat, shapeFlags);
            triMeshShape->setContactOffset(0.1f);
            triMeshShape->setRestOffset(0.02f);

            rbDynamic->attachShape(*triMeshShape);
            triMeshShape->release();

            PxReal density = 100.0f;
            PxRigidBodyExt::updateMassAndInertia(*rbDynamic, density);

            //physicsManager->dynamicRbs.push_back(rbDynamic);

            physicsManager->scene->addActor(*rbDynamic);

            rbDynamic->setSolverIterationCounts(50, 1);
            rbDynamic->setMaxDepenetrationVelocity(5.0f);
		}
		else {
            PxReal angle2 = 13 / 57.2958f; //in radians
            PxVec3 rot2(-1.0f, -1.0f, 0.0f); //normalized
            rbStatic = physicsManager->physics->createRigidStatic(PxTransform(PxVec3(0.0f, -4.0f, 5.0f), PxQuat(angle2, rot2)));
            rbStatic->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, true);

            /*rigidbody2->setLinearDamping(0.2f);
            rigidbody2->setAngularDamping(0.1f);

            rigidbody2->setRigidBodyFlag(PxRigidBodyFlag::eENABLE_GYROSCOPIC_FORCES, true);
            rigidbody2->setRigidBodyFlag(PxRigidBodyFlag::eENABLE_SPECULATIVE_CCD, true);*/



            /*PxTriangleMeshGeometry geom2(triangleMesh);
            geom2.scale = PxVec3(1.0f, 1.0f, 1.0f);*/


            PxShape* triMeshShape = PxRigidActorExt::createExclusiveShape(*rbStatic, PxBoxGeometry(1, 1, 1), *physicsManager->mat, shapeFlags);
            triMeshShape->setContactOffset(0.1f);
            triMeshShape->setRestOffset(0.02f);

            rbStatic->attachShape(*triMeshShape);


            //physicsManager->staticRbs.push_back(rbStatic);

            physicsManager->scene->addActor(*rbStatic);
		}
	}



    void UpdateMesh(Model* model) {
        // Collect mesh points from the model's vertices
        std::vector<PxVec3> meshPoints;
        for (int i = 0; i < model->vertices.size(); i++) {
            meshPoints.push_back(PxVec3(model->vertices[i].position.x, model->vertices[i].position.y, model->vertices[i].position.z));
        }

        // Create a mesh descriptor
        PxTriangleMeshDesc meshDesc;
        meshDesc.points.count = static_cast<PxU32>(meshPoints.size());
        meshDesc.points.stride = sizeof(PxVec3);
        meshDesc.points.data = meshPoints.data();

        meshDesc.triangles.count = static_cast<PxU32>(model->indices.size() / 3);
        meshDesc.triangles.stride = sizeof(PxU32) * 3;
        meshDesc.triangles.data = model->indices.data();

        // Cook the triangle mesh
        PxDefaultMemoryOutputStream writeBuffer;
        PxTriangleMeshCookingResult::Enum result;
        bool status = PxCookTriangleMesh(params, meshDesc, writeBuffer, &result);
        if (!status) {
            return;
        }

        PxDefaultMemoryInputData readBuffer(writeBuffer.getData(), writeBuffer.getSize());
        PxTriangleMesh* triangleMesh = physicsManager->physics->createTriangleMesh(readBuffer);

        PxTriangleMeshGeometry geom(triangleMesh);
        PxShape* triMeshShape = physicsManager->physics->createShape(geom, *physicsManager->mat, true, shapeFlags);

        if (isDynamic) {
            rbDynamic->attachShape(*triMeshShape);
        }
        else {
            rbStatic->attachShape(*triMeshShape);
        }

        triMeshShape->release();
    }

    

    void NewRB2D() {

        if (isDynamic) {
            b2BodyDef bodyDef;
            bodyDef.type = b2_dynamicBody;
            bodyDef.position.Set(0.0f, 10.0f);

            rb2D = physicsManager->scene2D.CreateBody(&bodyDef);

            b2PolygonShape dynamicBox;
            dynamicBox.SetAsBox(1.0f, 1.0f);

            b2FixtureDef fixtureDef;
            fixtureDef.shape = &dynamicBox;
            fixtureDef.density = 1.0f;
            fixtureDef.friction = 0.3f;

            rb2D->CreateFixture(&fixtureDef);
        }
        else {
            b2BodyDef groundBodyDef;
            groundBodyDef.type = b2_staticBody;
            groundBodyDef.position.Set(0.0f, -10.0f);

            b2Body* groundBody = physicsManager->scene2D.CreateBody(&groundBodyDef);

            b2PolygonShape groundBox;
            groundBox.SetAsBox(50.0f, 10.0f);

            groundBody->CreateFixture(&groundBox, 0.0f);
        }
    }


    Physics* physicsManager;

	PxRigidStatic* rbStatic = nullptr;
	PxRigidDynamic* rbDynamic = nullptr;

    PxShapeFlags shapeFlags;

    PxCookingParams params = PxCookingParams(PxTolerancesScale());


    //2D section

    b2Body* rb2D = nullptr;


	bool isDynamic;
};

