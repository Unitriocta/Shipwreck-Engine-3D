#include "testing.h"

#include "StartEngine.h"
using namespace EngineInstance;


//3D Physics from PhysX

bool Physics::SetupPhysics()
{

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


    physics = PxCreatePhysics(PX_PHYSICS_VERSION, *foundation, PxTolerancesScale(1.0f, 10.0f), recordMemoryAllocations, pvd);


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

    material = physics->createMaterial(0.5f, 0.5f, 0.1f); //static (fric), dynamic (fric), restitution (bounciness / recoil)
    if (!material) {
        //error
        return false;
    }


    bool selfCollisions = true;

    PxAggregateFilterHint hint = PxGetAggregateFilterHint(PxAggregateType::eGENERIC, selfCollisions);

    PxU32 maxActors = 30;
    PxAggregate* aggregate = physics->createAggregate(maxActors, maxActors, hint);



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

void Physics::onContact(PxContactPairHeader& pairHeader, const PxContactPair* pairs, PxU32 nbPairs)
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


void Physics::Simulate() {
    std::chrono::high_resolution_clock::time_point currentFrameTime = std::chrono::high_resolution_clock::now();

    float deltaTime = std::chrono::duration_cast<std::chrono::duration<float>>(currentFrameTime - lastFrameTime).count();

    //for (int i = 0; i < startEng.containers.size(); i++) {
    //    if (startEng.containers[i]->rigidbody.rbDynamic != nullptr/* && startEng.containers[i]->rigidbody.hasGravity*/) {
    //        startEng.containers[i]->rigidbody.applyGravityForce();
    //    }
    //}

    scene->simulate(deltaTime * 1.4f);
    scene->fetchResults();

    lastFrameTime = currentFrameTime;
}






//2D Physics from Box2D

bool Physics::SetupPhysics2D()
{
    /*b2BodyDef groundBodyDef;
    groundBodyDef.type = b2_staticBody;
    groundBodyDef.position.Set(0.0f, -10.0f);

    b2Body* groundBody = scene2D.CreateBody(&groundBodyDef);

    b2PolygonShape groundBox;
    groundBox.SetAsBox(50.0f, 10.0f);

    groundBody->CreateFixture(&groundBox, 0.0f);



    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(0.0f, 4.0f);

    b2Body* body = scene2D.CreateBody(&bodyDef);

    b2PolygonShape dynamicBox;
    dynamicBox.SetAsBox(1.0f, 1.0f);

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &dynamicBox;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.3f;

    body->CreateFixture(&fixtureDef);*/

    return true;
}

void Physics::Simulate2D() {
    std::chrono::high_resolution_clock::time_point currentFrameTime = std::chrono::high_resolution_clock::now();

    float deltaTime = std::chrono::duration_cast<std::chrono::duration<float>>(currentFrameTime - lastFrameTime).count();


    scene2D.Step((1.0f/60.0f) - (deltaTime), velocityIterations, positionIterations);
    scene2D.ClearForces();


    lastFrameTime = currentFrameTime;
}


bool Physics::B2DRayCheck(b2Fixture* fixture) {
    b2Vec2 p1(0.0f, 0.0f); // Starting point of the ray
    b2Vec2 p2(1.0f, 0.0f); // Ending point of the ray

    // Create a RayCast Input
    b2RayCastInput input;
    input.p1 = p1;
    input.p2 = p2;
    input.maxFraction = 1.0f;

    // Perform the RayCast
    b2RayCastOutput output;
    if (fixture->RayCast(&output, input, 0)) {
        // RayCast hit something
        //b2Vec2 hitPoint = p1 + output.fraction * (p2 - p1);
        //b2Vec2 hitNormal = output.normal;

        return true;
    }
    else {
        return false;
    }
}

bool Physics::RayCheck(PxActor* actor) {

    PxVec3 p1 = PxVec3(0.0f, 0.0f, 0.0f);
    PxVec3 dir = PxVec3(0.0f, 0.0f, 1.0f); //normalized
    PxReal dist = 10.0f;

    PxU32 totalDynamicActors = scene->getNbActors(PxActorTypeFlag::eRIGID_DYNAMIC);
    PxU32 totalStaticActors = scene->getNbActors(PxActorTypeFlag::eRIGID_STATIC);
    PxU32 maxHits = totalDynamicActors + totalStaticActors;
    std::vector<PxRaycastHit> touchingHits;
    touchingHits.reserve(maxHits);

    const PxHitFlags hitFlags = PxHitFlag::ePOSITION | PxHitFlag::eNORMAL;
    PxRaycastBuffer hitInfo(&touchingHits[0], maxHits);
    bool status = scene->raycast(p1, dir, dist, hitInfo, hitFlags);

    if (status) {
        for (PxU32 i = 0; i < hitInfo.getNbAnyHits(); ++i)
        {
            const PxRaycastHit& hit = hitInfo.getAnyHit(i);

            if (hit.actor == actor) {
                return true;
            }
        }
    }

    return false;
}