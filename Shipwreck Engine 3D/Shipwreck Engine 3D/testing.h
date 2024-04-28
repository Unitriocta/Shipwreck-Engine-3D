#pragma once

#include "physx/PxPhysicsAPI.h"
using namespace physx;


using namespace std;
#include <vector>

#include <chrono>


class Physics {
public:
    Physics() {
    }



    class PxAllocatorCallback
    {
    public:
        virtual ~PxAllocatorCallback() {}
        virtual void* allocate(size_t size, const char* typeName, const char* filename,
            int line) = 0;
        virtual void deallocate(void* ptr) = 0;
    };

    class UserErrorCallback : public PxErrorCallback
    {
    public:
        UserErrorCallback() {

        }
    public:
        virtual void reportError(PxErrorCode::Enum code, const char* message, const char* file,
            int line)
        {
            // error processing implementation
        }
    };



    class TaskWait : public PxLightCpuTask
    {
    public:

        TaskWait(PxSync* sync) : PxLightCpuTask(), mSync(sync) {}

        virtual void run()  PX_OVERRIDE
        {
        }

        PX_INLINE void release()    PX_OVERRIDE
        {
            PxLightCpuTask::release();
            mSync->set();
        }

        virtual const char* getName() const PX_OVERRIDE
        {
            return "TaskWait";
        }

    private:

        PxSync* mSync;
    };


    bool SetupPhysics() {

        static PxDefaultErrorCallback errorCallback;
        static PxDefaultAllocator allocatorCallback;


        bool recordMemoryAllocations = true;

        foundation = PxCreateFoundation(PX_PHYSICS_VERSION, allocatorCallback, errorCallback);
        if (!foundation)
        {
            //Error("PxCreateFoundation failed!");
            return false;
        }

        pvd = PxCreatePvd(*foundation);
        
        transport = PxDefaultPvdSocketTransportCreate("", 5425, 10);

        pvd->connect(*transport, PxPvdInstrumentationFlag::eALL);


        physics = PxCreatePhysics(PX_PHYSICS_VERSION, *foundation, PxTolerancesScale(1.0f,10.0f), recordMemoryAllocations, pvd);
        
        
        if (!physics)
        {
            //Error("PxCreatePhysics failed!");
            return false;
        }

        if (!PxInitExtensions(*physics, pvd))
        {
            //Error("PxInitExtensions failed!");
            return false;
        }

        PxSceneDesc sceneDesc(physics->getTolerancesScale());
        sceneDesc.gravity = PxVec3(0.0f, -10.0f, 0.0f);

        if (!sceneDesc.cpuDispatcher) {
            PxDefaultCpuDispatcher* cpuDispatcher = PxDefaultCpuDispatcherCreate(1);
            if (!cpuDispatcher)
            {
                return false;
            }
            sceneDesc.cpuDispatcher = cpuDispatcher;
        }
        if (!sceneDesc.filterShader)
        {
            PxSimulationFilterShader defaultFilterShader = PxDefaultSimulationFilterShader;
            sceneDesc.filterShader = defaultFilterShader;
        }

        sceneDesc.broadPhaseType = PxBroadPhaseType::ePABP; //collision sweeping



        scene = physics->createScene(sceneDesc);

        if (!scene) {
            return false;
        }

        scene->setVisualizationParameter(PxVisualizationParameter::eSCALE, 1.0f);
        scene->setVisualizationParameter(PxVisualizationParameter::eCOLLISION_SHAPES, 1.0f);




        bool selfCollisions = true;

        PxAggregateFilterHint hint = PxGetAggregateFilterHint(PxAggregateType::eGENERIC, selfCollisions);

        PxU32 maxActors = 30;
        PxAggregate* aggregate = physics->createAggregate(maxActors,maxActors, hint);





        mat = physics->createMaterial(0.5f, 0.5f, 0.1f); //static (fric), dynamic (fric), restitution (bounciness / recoil)
        if (!mat) {
            return false;
        }



        UserErrorCallback callback;


        const PxU32 threads = 8;
        PxDefaultCpuDispatcher* dispatcher = PxDefaultCpuDispatcherCreate(threads);
        taskManager = PxTaskManager::createTaskManager(callback, dispatcher);


        PxBroadPhaseDesc bpDesc(PxBroadPhaseType::ePABP);
        broadPhase = PxCreateBroadPhase(bpDesc);

        return true;
        //PxTolerancesScale scale;
        //scale.length = 100;        // typical length of an object
        //scale.speed = 981;         // typical speed of an object, gravity*1s is a reasonable choice
        //PxPhysics* p = PxCreatePhysics(PX_PHYSICS_VERSION, *foundation, scale);
    }



    //later have a function that takes in a mesh class
    bool TestPhysics() {
        vector<PxVec3> verts = 
        { 
          PxVec3(-1.0f, -1.0f, -1.0f),
          PxVec3(1.0f, -1.0f, -1.0f),
          PxVec3(-1.0f,  1.0f, -1.0f),
          PxVec3(1.0f,  1.0f, -1.0f),

          PxVec3(-1.0f, -1.0f,  1.0f),
          PxVec3(1.0f, -1.0f,  1.0f),
          PxVec3(-1.0f,  1.0f,  1.0f),
          PxVec3(1.0f,  1.0f,  1.0f)
        };
        
        
        vector<PxU32> indices =
        { 
            0,2,1, 2,3,1,
            1,3,5, 3,7,5,
            2,6,3, 3,6,7,
            4,5,7, 4,7,6,
            0,4,2, 2,4,6,
            0,1,4, 1,5,4

        };


        PxTolerancesScale scale;
        PxCookingParams params(scale);
        // disable mesh cleaning - perform mesh validation on development configurations
        params.meshPreprocessParams |= PxMeshPreprocessingFlag::eDISABLE_CLEAN_MESH;
        // disable edge precompute, edges are set for each triangle, slows contact generation
        params.meshPreprocessParams |= PxMeshPreprocessingFlag::eDISABLE_ACTIVE_EDGES_PRECOMPUTE;



        PxTriangleMeshDesc meshDesc;
        meshDesc.points.count = verts.size();
        meshDesc.points.stride = sizeof(PxVec3);
        meshDesc.points.data = verts.data();

        meshDesc.triangles.count = indices.size() / 3;
        meshDesc.triangles.stride = sizeof(PxU32) * 3;
        meshDesc.triangles.data = indices.data();



        PxTriangleMesh* triangleMesh = PxCreateTriangleMesh(params, meshDesc, physics->getPhysicsInsertionCallback() );

        

        PxShapeFlags shapeFlags = PxShapeFlag::eSCENE_QUERY_SHAPE | PxShapeFlag::eSIMULATION_SHAPE | PxShapeFlag::eVISUALIZATION;

        PxReal angle = 0/*40 / 57.2958f*/;
        PxVec3 rot(0.0f,0.0f,0.0f);
        rigidbody = physics->createRigidDynamic(PxTransform( PxVec3(0.0f, 26.0f, 5.0f), PxQuat(angle, rot) ));
        rigidbody->setLinearDamping(0.2f);
        rigidbody->setAngularDamping(0.1f);

        rigidbody->setRigidBodyFlag(PxRigidBodyFlag::eENABLE_GYROSCOPIC_FORCES, true);
        rigidbody->setRigidBodyFlag(PxRigidBodyFlag::eENABLE_SPECULATIVE_CCD, true);

        PxTriangleMeshGeometry geom(triangleMesh);
        geom.scale = PxVec3(1.0f, 1.0f, 1.0f);



        PxShape* triMeshShape = PxRigidActorExt::createExclusiveShape(*rigidbody, PxSphereGeometry(1), *mat, shapeFlags);
        //PxShape* triMeshShape = PxRigidActorExt::createExclusiveShape(*rigidbody, geom, *mat, shapeFlags);
        triMeshShape->setContactOffset(0.1f);
        triMeshShape->setRestOffset(0.02f);

        rigidbody->attachShape(*triMeshShape);
        triMeshShape->release();

        PxReal density = 100.0f;
        PxRigidBodyExt::updateMassAndInertia(*rigidbody, density);


        scene->addActor(*rigidbody); // ||  <--- ERROR <--- ||

        rigidbody->setSolverIterationCounts(50, 1);
        rigidbody->setMaxDepenetrationVelocity(5.0f);



        //split


        PxReal angle2 = 2 / 57.2958f; //in radians
        PxVec3 rot2(-1.0f, -1.0f, 0.0f); //normalized
        rigidbody2 = physics->createRigidStatic(PxTransform( PxVec3(0.0f, -4.0f, 5.0f), PxQuat(angle2, rot2) ));
        rigidbody2->setActorFlag(PxActorFlag::eDISABLE_GRAVITY,true);

        /*rigidbody2->setLinearDamping(0.2f);
        rigidbody2->setAngularDamping(0.1f);

        rigidbody2->setRigidBodyFlag(PxRigidBodyFlag::eENABLE_GYROSCOPIC_FORCES, true);
        rigidbody2->setRigidBodyFlag(PxRigidBodyFlag::eENABLE_SPECULATIVE_CCD, true);*/



        PxTriangleMeshGeometry geom2(triangleMesh);
        geom2.scale = PxVec3(1.0f, 1.0f, 1.0f);


        triMeshShape = PxRigidActorExt::createExclusiveShape(*rigidbody2, PxBoxGeometry(1, 1, 1), *mat, shapeFlags);
        //triMeshShape = PxRigidActorExt::createExclusiveShape(*rigidbody2, geom2, *mat, shapeFlags);
        triMeshShape->setContactOffset(0.1f);
        triMeshShape->setRestOffset(0.02f);

        rigidbody2->attachShape(*triMeshShape);




        scene->addActor(*rigidbody2); // ||  <--- ERROR <--- ||



        /*PxShapeFlags shapeFlags = PxShapeFlag::eVISUALIZATION | PxShapeFlag::eSCENE_QUERY_SHAPE | PxShapeFlag::eSIMULATION_SHAPE;


        PxMeshScale geomScale(PxVec3(1, 1, 1), PxQuat(PxIdentity));

        PxTriangleMeshGeometry geom(triangleMesh, geomScale);
        PxShape* myTriMeshShape = PxRigidActorExt::createExclusiveShape(*actor, geom, *mat, shapeFlags);*/

        return true;
    }
    

    virtual void onContact(PxContactPairHeader& pairHeader, const PxContactPair* pairs, PxU32 nbPairs)
    {
        // Retrieve the current poses and velocities of the two actors involved in the contact event.

        const PxTransform body0PoseAtEndOfSimulateStep = pairHeader.actors[0]->is<PxRigidDynamic>()->getGlobalPose();
        const PxTransform body1PoseAtEndOfSimulateStep = pairHeader.actors[1]->is<PxRigidDynamic>()->getGlobalPose();

        const PxVec3 body0LinVelAtEndOfSimulateStep = pairHeader.actors[0]->is<PxRigidDynamic>() ? pairHeader.actors[0]->is<PxRigidDynamic>()->getLinearVelocity() : PxVec3(PxZero);
        const PxVec3 body1LinVelAtEndOfSimulateStep = pairHeader.actors[0]->is<PxRigidDynamic>() ? pairHeader.actors[1]->is<PxRigidDynamic>()->getLinearVelocity() : PxVec3(PxZero);

        const PxVec3 body0AngVelAtEndOfSimulateStep = pairHeader.actors[0]->is<PxRigidDynamic>() ? pairHeader.actors[0]->is<PxRigidDynamic>()->getAngularVelocity() : PxVec3(PxZero);
        const PxVec3 body1AngVelAtEndOfSimulateStep = pairHeader.actors[0]->is<PxRigidDynamic>() ? pairHeader.actors[1]->is<PxRigidDynamic>()->getAngularVelocity() : PxVec3(PxZero);

        // Retrieve the poses and velocities of the two actors involved in the contact event as they were
        // when the contact event was detected.

        PxContactPairExtraDataIterator iter(pairHeader.extraDataStream, pairHeader.extraDataStreamSize);
        while (iter.nextItemSet())
        {
            const PxTransform body0PoseAtContactEvent = iter.eventPose->globalPose[0];
            const PxTransform body1PoseAtContactEvent = iter.eventPose->globalPose[1];

            const PxVec3 body0LinearVelocityAtContactEvent = iter.preSolverVelocity->linearVelocity[0];
            const PxVec3 body1LinearVelocityAtContactEvent = iter.preSolverVelocity->linearVelocity[1];

            const PxVec3 body0AngularVelocityAtContactEvent = iter.preSolverVelocity->angularVelocity[0];
            const PxVec3 body1AngularVelocityAtContactEvent = iter.preSolverVelocity->angularVelocity[1];
        }
    }

    /*
    gSensorActor->getShapes(&sensorShape, 1);
    sensorShape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, false);
    sensorShape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, true);
    */
    void OnTrigger(PxTriggerPair* pairs, PxU32 count)
    {
        for (PxU32 i = 0; i < count; i++)
        {
            // ignore pairs when shapes have been deleted
            if (pairs[i].flags & (PxTriggerPairFlag::eREMOVED_SHAPE_TRIGGER | PxTriggerPairFlag::eREMOVED_SHAPE_OTHER))
                continue;

            // Detect for example that a player entered a checkpoint zone
            if (pairs[i].otherShape->getActor() == NULL)
            {
                //if(pairs[i].triggerShape->getActor()){}
            }
        }
    }

    
    void Simulate() {
        std::chrono::high_resolution_clock::time_point currentFrameTime = std::chrono::high_resolution_clock::now();

        float deltaTime = std::chrono::duration_cast<std::chrono::duration<float>>(currentFrameTime - lastFrameTime).count();

        scene->simulate(deltaTime * 1.4f);
        scene->fetchResults();
        //rigidbody->addForce(PxVec3(0.0f,1.0f,0.0f),PxForceMode::eFORCE);

        lastFrameTime = currentFrameTime;
    }


public:
    PxPhysics* physics;

    PxFoundation* foundation;
    PxPvd* pvd;

    PxPvdTransport* transport;


    PxScene* scene;

    PxRigidDynamic* rigidbody;
    PxRigidStatic* rigidbody2;

    PxMaterial* mat;


    PxTaskManager* taskManager;

    PxBroadPhase* broadPhase;

private:
    std::chrono::high_resolution_clock::time_point lastFrameTime = std::chrono::high_resolution_clock::now();
};