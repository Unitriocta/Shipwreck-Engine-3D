#pragma once

#include "testing.h"
#include "Transform.h"

#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Rigidbody
{
public:

    Rigidbody(bool isDynamic_);

    Rigidbody();

    void NewRB(bool _isDynamic, bool _is2D) {
        
        rbStatic = nullptr;
        rbDynamic = nullptr;
        rb2D = nullptr;

        if (!_is2D) {
            params.meshPreprocessParams |= PxMeshPreprocessingFlag::eDISABLE_CLEAN_MESH;
            params.meshPreprocessParams |= PxMeshPreprocessingFlag::eDISABLE_ACTIVE_EDGES_PRECOMPUTE;

            shapeFlags = PxShapeFlag::eSCENE_QUERY_SHAPE | PxShapeFlag::eSIMULATION_SHAPE | PxShapeFlag::eVISUALIZATION;

            if (_isDynamic) {
                material = physicsManager->physics->createMaterial(0.5f, 0.5f, 0.1f); //static (fric), dynamic (fric), restitution (bounciness / recoil)
                if (!material) {
                    return;
                }

                rbDynamic = physicsManager->physics->createRigidDynamic(PxTransform(PxVec3(transform->globalPosition.x, transform->globalPosition.y, transform->globalPosition.z), PxQuat(transform->globalQuaternionRotation.x, transform->globalQuaternionRotation.y, transform->globalQuaternionRotation.z, transform->globalQuaternionRotation.w)));
                rbDynamic->setLinearDamping(0.2f);
                rbDynamic->setAngularDamping(0.1f);

                rbDynamic->setRigidBodyFlag(PxRigidBodyFlag::eENABLE_GYROSCOPIC_FORCES, true);
                rbDynamic->setRigidBodyFlag(PxRigidBodyFlag::eENABLE_SPECULATIVE_CCD, true);



                PxShape* triMeshShape = PxRigidActorExt::createExclusiveShape(*rbDynamic, PxSphereGeometry(1), *material, shapeFlags);
                triMeshShape->setContactOffset(0.1f);
                triMeshShape->setRestOffset(0.02f);

                rbDynamic->attachShape(*triMeshShape);
                triMeshShape->release();

                PxReal density = 100.0f;
                PxRigidBodyExt::updateMassAndInertia(*rbDynamic, density);

                physicsManager->scene->addActor(*rbDynamic);

                rbDynamic->setSolverIterationCounts(50, 1);
                rbDynamic->setMaxDepenetrationVelocity(5.0f);
            }
            else {
                material = physicsManager->physics->createMaterial(0.5f, 0.5f, 0.1f); //static (fric), dynamic (fric), restitution (bounciness / recoil)
                if (!material) {
                    //error
                    return;
                }

                PxReal angle2 = 13 / 57.2958f; //in radians
                PxVec3 rot2(-1.0f, -1.0f, 0.0f); //normalized
                rbStatic = physicsManager->physics->createRigidStatic(PxTransform(PxVec3(transform->globalPosition.x, transform->globalPosition.y, transform->globalPosition.z), PxQuat(transform->globalQuaternionRotation.x, transform->globalQuaternionRotation.y, transform->globalQuaternionRotation.z, transform->globalQuaternionRotation.w)));
                rbStatic->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, true);

                /*rigidbody2->setLinearDamping(0.2f);
                rigidbody2->setAngularDamping(0.1f);

                rigidbody2->setRigidBodyFlag(PxRigidBodyFlag::eENABLE_GYROSCOPIC_FORCES, true);
                rigidbody2->setRigidBodyFlag(PxRigidBodyFlag::eENABLE_SPECULATIVE_CCD, true);*/



                /*PxTriangleMeshGeometry geom2(triangleMesh);
                geom2.scale = PxVec3(1.0f, 1.0f, 1.0f);*/


                PxShape* triMeshShape = PxRigidActorExt::createExclusiveShape(*rbStatic, PxBoxGeometry(1, 1, 1), *material, shapeFlags);
                triMeshShape->setContactOffset(0.1f);
                triMeshShape->setRestOffset(0.02f);

                rbStatic->attachShape(*triMeshShape);


                //physicsManager->staticRbs.push_back(rbStatic);

                physicsManager->scene->addActor(*rbStatic);
            }
        }
        else {
            if (_isDynamic) {
                b2BodyDef bodyDef;
                bodyDef.type = b2_dynamicBody;
                bodyDef.position.Set(transform->globalPosition.x, transform->globalPosition.y);

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
                groundBodyDef.position.Set(transform->globalPosition.x, transform->globalPosition.y);

                b2Body* groundBody = physicsManager->scene2D.CreateBody(&groundBodyDef);

                b2PolygonShape groundBox;
                groundBox.SetAsBox(50.0f, 10.0f);

                groundBody->CreateFixture(&groundBox, 0.0f);
            }
        }
	}

    
    void AddForce(Vec3 force) {

        if (rbDynamic != nullptr) {
            rbDynamic->addForce(PxVec3(force.x, force.y, force.z));
        }
        else if (rbStatic != nullptr) {

        }
        else if (rb2D != nullptr) {

        }
    }



    void UpdateMesh(Model* model) {

        if (rbDynamic != nullptr) {

            PxU32 shapeCount = rbDynamic->getNbShapes();

            // Create an array to hold the shape pointers
            PxShape** shapes = new PxShape*[shapeCount];

            rbDynamic->getShapes(shapes, shapeCount);

            rbDynamic->detachShape(*shapes[0]);

            delete[] shapes;
        }
        else if (rbStatic != nullptr) {
            
            PxU32 shapeCount = rbStatic->getNbShapes();

            // Create an array to hold the shape pointers
            PxShape** shapes = new PxShape*[shapeCount];

            rbStatic->getShapes(shapes, shapeCount);

            rbStatic->detachShape(*shapes[0]);

            delete[] shapes;
        }


        std::vector<PxVec3> meshPoints;


        PxVec3 pxScale;

        if (transform != nullptr) {
            transform->updateGlobalProperties();

            pxScale = PxVec3(transform->globalScale.x, transform->globalScale.y, transform->globalScale.z);
        }
        else {
            pxScale = PxVec3(1.0f, 1.0f, 1.0f);
        }

        //pxScale = PxVec3(1.0f, 1.0f, 1.0f);
        for (int i = 0; i < model->vertices.size(); i++) {
            meshPoints.push_back(PxVec3(model->vertices[i].position.x * pxScale.x, model->vertices[i].position.y * pxScale.y, model->vertices[i].position.z * pxScale.z));
        }

        PxTriangleMeshDesc meshDesc;
        meshDesc.points.count = static_cast<PxU32>(meshPoints.size());
        meshDesc.points.stride = sizeof(PxVec3);
        meshDesc.points.data = meshPoints.data();

        meshDesc.triangles.count = static_cast<PxU32>(model->indices.size() / 3);
        meshDesc.triangles.stride = sizeof(PxU32) * 3;
        meshDesc.triangles.data = model->indices.data();

        PxDefaultMemoryOutputStream writeBuffer;
        PxTriangleMeshCookingResult::Enum result;
        bool status = PxCookTriangleMesh(params, meshDesc, writeBuffer, &result);
        if (!status) {
            return;
        }

        PxDefaultMemoryInputData readBuffer(writeBuffer.getData(), writeBuffer.getSize());
        PxTriangleMesh* triangleMesh = physicsManager->physics->createTriangleMesh(readBuffer);

        PxTriangleMeshGeometry geom(triangleMesh);
        PxShape* triMeshShape = physicsManager->physics->createShape(geom, *material, true, shapeFlags);

        if (rbDynamic != nullptr) {
            rbDynamic->attachShape(*triMeshShape);
        }
        else if (rbStatic != nullptr) {
            rbStatic->attachShape(*triMeshShape);
        }

        triMeshShape->release();
    }


    void SetTransform(Transform* _transform) {

        if (_transform != nullptr) {
            SetPosition(_transform->globalPosition);
            SetRotation(_transform->globalRotation);
            SetScale(_transform->globalScale);
        }
    }
    void SetPosition(Vec3 newPosition) {

        if (rbDynamic != nullptr) {
            rbDynamic->setGlobalPose(PxTransform(PxVec3(newPosition.x, newPosition.y, newPosition.z), rbDynamic->getGlobalPose().q));
        }
        else if (rbStatic != nullptr) {
            rbStatic->setGlobalPose(PxTransform(PxVec3(newPosition.x, newPosition.y, newPosition.z), rbStatic->getGlobalPose().q));
        }
        else if (rb2D != nullptr) {
            rb2D->SetTransform(b2Vec2(newPosition.x, newPosition.y), rb2D->GetAngle());
        }
    }
    void SetRotation(Vec3 newRotation) {

        if (rbDynamic != nullptr) {
            glm::quat newQuat = glm::quat(glm::radians(glm::vec3(newRotation.x, newRotation.y, newRotation.z)));
            rbDynamic->setGlobalPose(PxTransform(rbDynamic->getGlobalPose().p, PxQuat(newQuat.x, newQuat.y, newQuat.z, newQuat.w)));
        }
        else if (rbStatic != nullptr) {
            glm::quat newQuat = glm::quat(glm::radians(glm::vec3(newRotation.x, newRotation.y, newRotation.z)));
            rbStatic->setGlobalPose(PxTransform(rbStatic->getGlobalPose().p, PxQuat(newQuat.x, newQuat.y, newQuat.z, newQuat.w)));
        }
        else if (rb2D != nullptr) {
            rb2D->SetTransform(b2Vec2(rb2D->GetPosition().x, rb2D->GetPosition().y), newRotation.z);
        }
    }
    void SetScale(Vec3 newScale) {

        if (rbDynamic != nullptr) {
            
            PxShape* shapes[1];
            rbDynamic->getShapes(shapes, 1);
            PxShape* oldShape = shapes[0];

            // Get the geometry of the old shape
            PxGeometryHolder geometryHolder = oldShape->getGeometry();

            // Remove the old shape from the rigidbody
            rbDynamic->detachShape(*oldShape);

            // Create a new shape with the scaled geometry
            PxShape* newShape = nullptr;
            switch (geometryHolder.getType()) {
            case PxGeometryType::eBOX:
            {
                PxBoxGeometry boxGeom = geometryHolder.box();
                boxGeom.halfExtents = boxGeom.halfExtents.multiply(PxVec3(newScale.x, newScale.y, newScale.z));
                newShape = physicsManager->physics->createShape(boxGeom, *material);
                break;
            }
            case PxGeometryType::eSPHERE:
            {
                PxSphereGeometry sphereGeom = geometryHolder.sphere();
                sphereGeom.radius *= newScale.x; // Assuming uniform scale
                newShape = physicsManager->physics->createShape(sphereGeom, *material);
                break;
            }
            case PxGeometryType::eCAPSULE:
            {
                PxCapsuleGeometry capsuleGeom = geometryHolder.capsule();
                capsuleGeom.radius *= newScale.x; // Assuming uniform scale
                capsuleGeom.halfHeight *= newScale.y; // Assuming non-uniform scale for height
                newShape = physicsManager->physics->createShape(capsuleGeom, *material);
                break;
            }
            case PxGeometryType::eCONVEXMESH:
            {
                PxShape* shapes[1];
                rbDynamic->getShapes(shapes, 1);
                PxShape* oldShape = shapes[0];

                // Get the geometry of the old shape
                PxConvexMeshGeometry oldGeom;
                bool status = oldShape;
                if (!status) return; // Ensure the shape is indeed a convex mesh

                PxConvexMesh* oldConvexMesh = oldGeom.convexMesh;
                
                // Retrieve the vertices from the old convex mesh
                PxU32 vertexCount = oldConvexMesh->getNbVertices();
                const PxVec3* vertices = oldConvexMesh->getVertices();

                // Scale the vertices
                std::vector<PxVec3> scaledVertices(vertexCount);

                PxVec3 pxScale = PxVec3(newScale.x, newScale.y, newScale.z);
                for (PxU32 i = 0; i < vertexCount; ++i) {
                    scaledVertices[i] = vertices[i].multiply(pxScale);
                }

                // Create a new convex mesh with the scaled vertices
                PxConvexMeshDesc convexDesc;
                convexDesc.points.count = vertexCount;
                convexDesc.points.stride = sizeof(PxVec3);
                convexDesc.points.data = scaledVertices.data();
                convexDesc.flags = PxConvexFlag::eCOMPUTE_CONVEX;
               
                PxDefaultMemoryOutputStream writeBuffer;
                PxConvexMeshCookingResult::Enum result;
                bool cookStatus = PxCookConvexMesh(params, convexDesc, writeBuffer, &result);
                if (!cookStatus) {
                    return; // Handle error appropriately
                }

                PxDefaultMemoryInputData readBuffer(writeBuffer.getData(), writeBuffer.getSize());
                PxConvexMesh* newConvexMesh = physicsManager->physics->createConvexMesh(readBuffer);

                // Create a new shape with the new convex mesh
                PxShape* newShape = physicsManager->physics->createShape(PxConvexMeshGeometry(newConvexMesh), *material);

                // Replace the old shape with the new shape
                rbDynamic->detachShape(*oldShape);
                rbDynamic->attachShape(*newShape);

                // Release the old shape and convex mesh
                oldShape->release();
                oldConvexMesh->release();
                break;
            }
            case PxGeometryType::eTRIANGLEMESH:
            {
                PxShape* shapes[1];
                rbDynamic->getShapes(shapes, 1);
                PxShape* oldShape = shapes[0];

                // Get the geometry of the old shape
                PxTriangleMeshGeometry oldGeom;
                bool status = oldShape;
                if (!status) return; // Ensure the shape is indeed a triangle mesh

                PxTriangleMesh* oldTriangleMesh = oldGeom.triangleMesh;

                // Retrieve the vertices from the old triangle mesh
                PxU32 vertexCount = oldTriangleMesh->getNbVertices();
                const PxVec3* vertices = oldTriangleMesh->getVertices();
                PxU32 indexCount = oldTriangleMesh->getNbTriangles();
                const void* indices = oldTriangleMesh->getTriangles();
                bool has16BitIndices = oldTriangleMesh->getTriangleMeshFlags() & PxTriangleMeshFlag::e16_BIT_INDICES;

                // Scale the vertices
                std::vector<PxVec3> scaledVertices(vertexCount);
                PxVec3 pxScale = PxVec3(newScale.x, newScale.y, newScale.z);
                for (PxU32 i = 0; i < vertexCount; ++i) {
                    scaledVertices[i] = vertices[i].multiply(pxScale);
                }

                // Create a new triangle mesh with the scaled vertices
                PxTriangleMeshDesc meshDesc;
                meshDesc.points.count = vertexCount;
                meshDesc.points.stride = sizeof(PxVec3);
                meshDesc.points.data = scaledVertices.data();
                meshDesc.triangles.count = indexCount;
                meshDesc.triangles.stride = has16BitIndices ? sizeof(PxU16) * 3 : sizeof(PxU32) * 3;
                meshDesc.triangles.data = indices;

                PxDefaultMemoryOutputStream writeBuffer;
                PxTriangleMeshCookingResult::Enum result;

                bool cookStatus = PxCookTriangleMesh(params, meshDesc, writeBuffer, &result);
                if (!cookStatus) {
                    return; // Handle error appropriately
                }

                PxDefaultMemoryInputData readBuffer(writeBuffer.getData(), writeBuffer.getSize());
                PxTriangleMesh* newTriangleMesh = physicsManager->physics->createTriangleMesh(readBuffer);

                // Create a new shape with the new triangle mesh
                PxShape* newShape = physicsManager->physics->createShape(PxTriangleMeshGeometry(newTriangleMesh), *material);

                // Replace the old shape with the new shape
                rbDynamic->detachShape(*oldShape);
                rbDynamic->attachShape(*newShape);

                // Release the old shape and triangle mesh
                oldShape->release();
                oldTriangleMesh->release();
                break;
            }
            // Handle other geometry types as necessary
            default:
                break;
            }

            if (newShape) {
                rbDynamic->attachShape(*newShape);

                oldShape->release();
            }
        }
        else if (rbStatic != nullptr) {
            PxShape* shapes[1];
            rbStatic->getShapes(shapes, 1);
            PxShape* oldShape = shapes[0];

            PxGeometryHolder geometryHolder = oldShape->getGeometry();

            rbStatic->detachShape(*oldShape);

            PxShape* newShape = nullptr;
            switch (geometryHolder.getType()) {
            case PxGeometryType::eBOX:
            {
                PxBoxGeometry boxGeom = geometryHolder.box();
                boxGeom.halfExtents = boxGeom.halfExtents.multiply(PxVec3(newScale.x, newScale.y, newScale.z));
                newShape = physicsManager->physics->createShape(boxGeom, *material);
                break;
            }
            case PxGeometryType::eSPHERE:
            {
                PxSphereGeometry sphereGeom = geometryHolder.sphere();
                sphereGeom.radius *= newScale.x;
                newShape = physicsManager->physics->createShape(sphereGeom, *material);
                break;
            }
            case PxGeometryType::eCAPSULE:
            {
                PxCapsuleGeometry capsuleGeom = geometryHolder.capsule();
                capsuleGeom.radius *= newScale.x;
                capsuleGeom.halfHeight *= newScale.y;
                newShape = physicsManager->physics->createShape(capsuleGeom, *material);
                break;
            }
            case PxGeometryType::eCONVEXMESH:
            {
                PxConvexMesh* oldConvexMesh = geometryHolder.convexMesh().convexMesh;

                PxU32 vertexCount = oldConvexMesh->getNbVertices();
                const PxVec3* vertices = oldConvexMesh->getVertices();

                std::vector<PxVec3> scaledVertices(vertexCount);

                PxVec3 pxScale = PxVec3(newScale.x, newScale.y, newScale.z);
                for (PxU32 i = 0; i < vertexCount; ++i) {
                    scaledVertices[i] = vertices[i].multiply(pxScale);
                }

                PxConvexMeshDesc convexDesc;
                convexDesc.points.count = vertexCount;
                convexDesc.points.stride = sizeof(PxVec3);
                convexDesc.points.data = scaledVertices.data();
                convexDesc.flags = PxConvexFlag::eCOMPUTE_CONVEX;

                PxDefaultMemoryOutputStream writeBuffer;
                PxConvexMeshCookingResult::Enum result;
                bool cookStatus = PxCookConvexMesh(params, convexDesc, writeBuffer, &result);
                if (!cookStatus) {
                    return;
                }

                PxDefaultMemoryInputData readBuffer(writeBuffer.getData(), writeBuffer.getSize());
                PxConvexMesh* newConvexMesh = physicsManager->physics->createConvexMesh(readBuffer);

                PxShape* newShape = physicsManager->physics->createShape(PxConvexMeshGeometry(newConvexMesh), *material);

                rbStatic->detachShape(*oldShape);
                rbStatic->attachShape(*newShape);

                oldShape->release();
                oldConvexMesh->release();
                break;
            }
            case PxGeometryType::eTRIANGLEMESH:
            {
                PxTriangleMesh* oldTriangleMesh = geometryHolder.triangleMesh().triangleMesh;

                PxU32 vertexCount = oldTriangleMesh->getNbVertices();
                const PxVec3* vertices = oldTriangleMesh->getVertices();
                PxU32 indexCount = oldTriangleMesh->getNbTriangles();
                const void* indices = oldTriangleMesh->getTriangles();
                bool has16BitIndices = oldTriangleMesh->getTriangleMeshFlags() & PxTriangleMeshFlag::e16_BIT_INDICES;

                std::vector<PxVec3> scaledVertices(vertexCount);
                PxVec3 pxScale = PxVec3(newScale.x, newScale.y, newScale.z);
                for (PxU32 i = 0; i < vertexCount; ++i) {
                    scaledVertices[i] = vertices[i].multiply(pxScale);
                }

                PxTriangleMeshDesc meshDesc;
                meshDesc.points.count = vertexCount;
                meshDesc.points.stride = sizeof(PxVec3);
                meshDesc.points.data = scaledVertices.data();
                meshDesc.triangles.count = indexCount;
                meshDesc.triangles.stride = has16BitIndices ? sizeof(PxU16) * 3 : sizeof(PxU32) * 3;
                meshDesc.triangles.data = indices;

                PxDefaultMemoryOutputStream writeBuffer;
                PxTriangleMeshCookingResult::Enum result;

                bool cookStatus = PxCookTriangleMesh(params, meshDesc, writeBuffer, &result);
                if (!cookStatus) {
                    return;
                }

                PxDefaultMemoryInputData readBuffer(writeBuffer.getData(), writeBuffer.getSize());
                PxTriangleMesh* newTriangleMesh = physicsManager->physics->createTriangleMesh(readBuffer);

                newShape = physicsManager->physics->createShape(PxTriangleMeshGeometry(newTriangleMesh), *material);


                oldShape->release();
                oldTriangleMesh->release();
                break;
            }
            default:
                break;
            }

            if (newShape) {
                rbStatic->attachShape(*newShape);

                oldShape->release();
            }
        }
        else if (rb2D != nullptr) {

        }
    }


    void SetVelocity(Vec3 _newVelocity) {
        if (rbDynamic != nullptr) {
            rbDynamic->setLinearVelocity(PxVec3(_newVelocity.x, _newVelocity.y, _newVelocity.z));
        }
        else if (rbStatic != nullptr) {
            //rbStatic->setLinearVelocity(PxVec3(_newVelocity.x, _newVelocity.y, _newVelocity.z));
        }
        else if (rb2D != nullptr) {

            rb2D->SetLinearVelocity(b2Vec2(_newVelocity.x, _newVelocity.y));
        }
    }

    Vec3 GetVelocity() {
        if (rbDynamic != nullptr) {
            PxVec3 pxVel = rbDynamic->getLinearVelocity();
            return Vec3(pxVel.x, pxVel.y, pxVel.z);
        }
        else if (rbStatic != nullptr) {
        }
        else if (rb2D != nullptr) {

            b2Vec2 b2Vel = rb2D->GetLinearVelocity();
            return Vec3(b2Vel.x, b2Vel.y, 0.0f);
        }
    }


    Physics* physicsManager;

	PxRigidStatic* rbStatic = nullptr;
	PxRigidDynamic* rbDynamic = nullptr;

    PxShapeFlags shapeFlags;

    PxCookingParams params = PxCookingParams(PxTolerancesScale());


    //2D section

    b2Body* rb2D = nullptr;

    PxMaterial* material;
    
private:
	bool isDynamic;
    bool is2D;
   
public:

    const bool& getDynamic() const { return isDynamic; }
    const bool& get2D() const { return is2D; }

    Transform* transform = nullptr;

    PxVec3 customGravityForce = PxVec3(0.0f, -10.0f, 0.0f);

    bool hasGravity = true;
    
    // Set actual scene gravity to PxVec3(0.0f, 0.0f, 0.0f), 
    // Update all rigidbody's using startEng.containers[i].rigidbody.rbDynamic.applyCustomGravity(), right before we simualte
    void applyGravityForce() {
        
        // F = m * g
        PxVec3 gravityForce = rbDynamic->getMass() * customGravityForce;

        rbDynamic->addForce(gravityForce, PxForceMode::eACCELERATION);
    }
};

