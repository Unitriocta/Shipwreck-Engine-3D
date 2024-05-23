/*
* Useful links for Game Engine Development
* Notes to self:
* 
* For extra windows to render, like progress for engine bootup: https://www.youtube.com/watch?v=D-PC-huX-l8
* 
* 
* 
* 
* 
* 
* 
* 
*/



#include "StartEngine.h"
using namespace EngineInstance;

#include <dxtex/DirectXTex.h>







#include "testing.h"


namespace EngineInstance {

    StartEngine startEng;

    HWND hWnd;

    bool isD3D = true;
}



#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // Current instance of the app opened
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // The main window class name

// Forward declarations of functions included in this code module:
ATOM         MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE hInstance, int nCmdShow, Vec2 windSize);
LRESULT CALLBACK      WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);



FontCreation::SetupFace *setupFace;
Sprite sprite;

Textbox newText;

Camera camera;
Transform camTransform;

D3DTexture texLoader;

Physics physics;

Button button;

Container container;
Container container2;
Container container3;

std::vector<Rigidbody> loadedRigidbodies;
std::vector<Model> loadedModels;
std::vector<Container> loadedContainers;


ModelImporter modelImporter;

GLFWwindow* window;

bool exited = false;

int targetFPS = 250;
std::chrono::milliseconds targetFrameLength(1000 / targetFPS);

const float sensitivity = 0.1f;

const std::string engineDataPath = "D:/Shipwreck Engine 3D/Shipwreck Engine 3D/data.json";

void DisplayNumAsTitle(float newVar);
void DisplayNumAsTitle(float newVar) {
    
    std::ostringstream oss;
    oss << "variable: " << newVar;


    std::string widthStr = oss.str();

    //MessageBoxA(hWnd, widthStr.c_str(), "Hey", 0);
    SetWindowTextA(hWnd, widthStr.c_str());
}

void DisplayStringAsTitle(std::string newVar);
void DisplayStringAsTitle(std::string newVar) {

    std::ostringstream oss;
    oss << "variable: " << newVar;


    std::string widthStr = oss.str();

    //MessageBoxA(hWnd, widthStr.c_str(), "Hey", 0);
    SetWindowTextA(hWnd, widthStr.c_str());
}


std::string RemoveQuotes(std::string& inString);


//WM_KEYDOWN
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {

    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);


    // Initialize global strings
    LoadString(hInstance, APP_NAME, szTitle, MAX_LOADSTRING);
    LoadString(hInstance, PROJ_FILENAME, szWindowClass, MAX_LOADSTRING);


    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance(hInstance, nCmdShow, Vec2(900,900)))
    {
        return FALSE;
    }

    startEng.timeManager.StartTime();



    setupFace = &startEng.Font().LoadFont("C:/Windows/Fonts/Arial.ttf");
    //setupFace = &startEng.Font().LoadFont("D:/Shipwreck Engine 3D/Shipwreck Engine 3D/Myfont-Regular.ttf");


    ConsoleSetup(setupFace, hWnd);

    std::string loadText = "abcdefghijklmnopqrstovwxyz";

    if (!physics.SetupPhysics()) {
        WPARAM wp = 1111;
        LPARAM lp = 1000;
        SendMessageA(hWnd, Msgbox, wp, lp);
    }
    /*rigidbody.NewRB();
    rigidbody2.NewRB();
    rigidbody3.NewRB2D();*/

    /*button.CreateFixture(rigidbody3.rb2D);
    if (physics.RayCheck(button.fixture)) {
        return 1;
    }*/

    sprite.LoadTexture("C:/Users/smsal/OneDrive/Documents/Game Art/Mask Game/Character/Character-1.png");
    //newTexture.LoadTexture("C:/Users/smsal/Downloads/slime-test-free-texture/textures/Depth_basecolor.png");

    D3DTexture loader;

    //modelImporter.ImportModel("C:/Users/smsal/OneDrive/Documents/Blender Modules/sphere.obj");
    //modelImporter.ImportModel("C:/Users/smsal/OneDrive/Documents/Blender Modules/sphereV2.fbx", &loader);

    modelImporter.ImportModel("C:/Users/smsal/Downloads/slime-test-free-texture/source/Scene 3/Scene 3.obj", &loader);
    modelImporter.ImportModel("C:/Users/smsal/OneDrive/Documents/Blender Modules/cube.fbx", &loader);





    startEng.containers.push_back(&container2);
    startEng.containers.push_back(&container);
    startEng.containers.push_back(&container3);


    container.name  = "Ball";
    container2.name = "Ground";
    container3.name = "PlayerSprite";

    container.models.AddModel(modelImporter.meshes[0]);
    container.models.modelList[0].modelPath = "C:/Users/smsal/OneDrive/Documents/Blender Modules/sphereV2.fbx";

    container2.models.AddModel(modelImporter.meshes[1]);
    container2.models.modelList[0].modelPath = "C:/Users/smsal/OneDrive/Documents/Blender Modules/cube.fbx";

    container3.sprites.AddSprite(&sprite);
    
    container.rigidbody = Rigidbody(&physics, true);
    container2.rigidbody = Rigidbody(&physics, false);
    container3.rigidbody = Rigidbody(&physics, true);


    container.rigidbody.NewRB();
    container2.rigidbody.NewRB();
    container3.rigidbody.NewRB2D();
    

    container.models.modelList[0].hasDiffuse = true;
    
    std::string narrowString = "C:/Users/smsal/Downloads/slime-test-free-texture/textures/Depth_basecolor.png";

    ID3D11ShaderResourceView* srv = nullptr;
    loader.LoadTextureFromFile(startEng.D3DGfx().device, narrowString, &srv);

    container.models.modelList[0].textures.diffuseTex = srv;





    //container.rigidbody->UpdateMesh(container.models.modelList[0]);
    //container2.rigidbody->UpdateMesh(container2.models.modelList[0]);


    Connection sendAddr;
    startEng.networkManager.AssignSendAddress(&sendAddr, "192.168.4.156");
    //sendAddr.SeparateFullAddress(startEng.networkManager.DomainToIP("0.tcp.ngrok.io") + ":18183");
    //sendAddr.SeparateFullAddress(startEng.networkManager.DomainToIP("smsaliou5935-35752.portmap.host") + ":35752");
    startEng.networkManager.transformAddresses.insert({0, &container.transform});
    startEng.networkManager.sendingAddresses.push_back(sendAddr);


    startEng.RenderFrame();
    
    
    
    //{Transform(0.000000,-4.000000,5.000000,0.025306,0.974694,-0.222105,0.000000,0.000000,0.000000),Rigidbody(f,f),Model("C:/Users/smsal/OneDrive/Documents/Blender Modules/cube.fbx"),Name("Ground")},{Transform(0.000000,16.000000,5.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000),Rigidbody(t,f),Model("C:/Users/smsal/OneDrive/Documents/Blender Modules/sphereV2.fbx"),Name("Ball")},{Transform(0.000000,9.997253,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000),Rigidbody(t,t),Name("PlayerSprite")}
    startEng.LoadEngine();
    startEng.SaveEngine();
    //startEng.LoadEngine();




    //Hides mouse and centers it so the delta doesn't skyrocket the moment the loop starts
    if (isD3D) {
        if (!startEng.mouse.mouseShown) {
            startEng.mouse.UpdateMouse(); //updates scroll wheel and mouse move delta
            startEng.mouse.D3DHideMouse();
            startEng.mouse.D3DCenterMouse(hWnd);
        }
    }
    else {

        if (!startEng.mouse.mouseShown) {
            startEng.mouse.UpdateMouse(); //updates scroll wheel and mouse move delta
            startEng.mouse.GLHideMouse(window);
            startEng.mouse.GLCenterMouse(window);
        }
    }


    MSG msg;

    /*std::thread renderThread(&StartEngine::RenderFrame, &startEng);
    std::thread inputThread(&StartEngine::HandleInput, &startEng);
    
    renderThread.join();
    inputThread.join();*/

    // Main message loop:
    while (!exited && !glfwWindowShouldClose(window))
    {
        auto frameStart = std::chrono::high_resolution_clock::now();
        /*
        if (GetMessage(&msg, nullptr, 0, 0) <= 0) {

        }*/
        if (isD3D && PeekMessage(&msg, hWnd, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        if (!isD3D) {
            glfwPollEvents();
        }
        startEng.networkManager.StartRecieveThread(startEng.networkManager);
        //container.rigidbody->rbDynamic->setGlobalPose(PxTransform(startEng.networkManager.transformAddresses[0]->position.x, startEng.networkManager.transformAddresses[0]->position.y, startEng.networkManager.transformAddresses[0]->position.z, PxQuat(0, 0, 0, 0)));


        if (!startEng.mouse.IsEmpty()) {
            /* const auto mouseEvent = startEng.mouse.ReadMouse();

             if (mouseEvent.GetMouseState() == Mouse::MouseEvent::State::Button0Down) {
                 SendMessageA(hWnd, Msgbox, 0, 0);
             }*/

            if (startEng.mouse.Button0) {

                startEng.mouse.mouseShown = false;

                if (isD3D) {
                    startEng.mouse.D3DHideMouse();
                    startEng.mouse.D3DCenterMouse(hWnd);
                    startEng.mouse.UpdateMouse(); //updates scroll wheel delta,   MAYBE: add a mouse movement delta, to add support for mouse movement per frame
                }
                else {
                    startEng.mouse.GLHideMouse(window);
                    startEng.mouse.GLCenterMouse(window);
                    startEng.mouse.UpdateMouse(); //updates scroll wheel delta,   MAYBE: add a mouse movement delta, to add support for mouse movement per frame
                }
            }

            if (startEng.mouse.Button1) {
                WPARAM wp = 1;
                LPARAM lp = 1;

                startEng.networkManager.StartMessageThread((char*)"SetPlayerPosition:7:5:7:0:", 0);
                //SendMessage(hWnd, Msgbox, wp, lp);
            }

            if (startEng.mouse.scrollWheel == 1.0f) {
                WPARAM wp = 3;
                LPARAM lp = 3;
                SendMessage(hWnd, Msgbox, wp, lp);
            }
            if (startEng.mouse.scrollWheel == -1.0f) {
                WPARAM wp = 4;
                LPARAM lp = 4;
                SendMessage(hWnd, Msgbox, wp, lp);
            }


            if (!startEng.mouse.inWindow) {
                WPARAM wp = 5;
                LPARAM lp = 5;
                //SendMessage(hWnd, Msgbox, wp, lp);
            }

        }


        startEng.RenderFrame();



        if (isD3D) {
            if (startEng.keyboard.GetKeyState(VK_ESCAPE)) {
                startEng.mouse.mouseShown = true;
            }



            if (!startEng.mouse.mouseShown) {
                startEng.mouse.D3DHideMouse();
                startEng.mouse.D3DCenterMouse(hWnd);
                startEng.mouse.UpdateMouse(); //updates scroll wheel and mouse move delta
            }
            else {
                startEng.mouse.UpdateMouse(); //updates scroll wheel delta,   MAYBE: add a mouse movement delta, to add support for mouse movement per frame
                startEng.mouse.D3DShowMouse();
            }
        }
        else {

            if (startEng.keyboard.GetKeyState(GLFW_KEY_ESCAPE)) {
                startEng.mouse.mouseShown = true;
            }

            if (!startEng.mouse.mouseShown) {
                startEng.mouse.GLHideMouse(window);
                startEng.mouse.GLCenterMouse(window);
                startEng.mouse.UpdateMouse(); //updates scroll wheel and mouse move delta
            }
            else {
                startEng.mouse.UpdateMouse(); //updates scroll wheel delta,   MAYBE: add a mouse movement delta, to add support for mouse movement per frame
                startEng.mouse.GLShowMouse(window);
            }
        }



        auto frameEnd = std::chrono::high_resolution_clock::now();
        auto frameDuration = frameEnd - frameStart;

        if (frameDuration < targetFrameLength) {
            std::this_thread::sleep_for(targetFrameLength - frameDuration);
        }




        //use '[key letter]' for letters
        //use VK_[key name] for accelerator keys like [CTRL], [SPACE], and [ALT]
    }



    if (!isD3D) {
        //glfwDestroyWindow(window);
        glDeleteVertexArrays(1, &startEng.GLGfx().VAO);
        glDeleteBuffers(1, &startEng.GLGfx().VBO);
        glDeleteBuffers(1, &startEng.GLGfx().EBO);
        glDeleteBuffers(1, &startEng.GLGfx().FBO);
        glDeleteProgram(startEng.GLGfx().shaderProgram);

        glfwTerminate();
    }
    return (int)msg.wParam;
}

void StartEngine::RenderFrame() {


    /*for (int i = 0; i < containers.size(); i++) {
        for (int j = 0; j < containers[i]->scripts.size(); j++) {

            containers[i]->scripts[j]->Update(timeManager._time, containers[i]);
        }
    }*/



    if (isD3D) {
        D3DGfx().ClearBuffer(0.2f, 0.2f, 0.7f);
        //Gfx().DrawRect(Vec2(0.0f, 0.0f), Vec2(1.0f, 1.0f), 19, 0.5f /*0.2f*/, Color(30, 30, 30, 255), hWnd);
        float rotTimer = startEng.timeManager.GetTime() / 1000;




        RECT rect;
        GetWindowRect(hWnd, &rect);
        int width = (rect.right - rect.left);
        int height = (rect.bottom - rect.top);



        physics.Simulate();
        physics.Simulate2D();


        //PxActor** actors;
        //physics.scene->getActors(PxActorTypeFlag::eRIGID_DYNAMIC,actors,1); //actors[0]->is<PxRigidDynamic>()->getGlobalPose().p.z





        //FPS camera

        camTransform.rotation.x += startEng.mouse.mouseDelta.y * sensitivity;
        camTransform.rotation.y += startEng.mouse.mouseDelta.x * sensitivity;

        camTransform.rotation.x = std::max(std::min(camTransform.rotation.x, 89.0f), -89.0f);

        // Implement rotation around Z for VR
        // rotationZ += mouse.deltaZ * sensitivity; // Uncomment if you have deltaZ

        camTransform.rotation = startEng.NormalizeAngles(camTransform.rotation);

        if (!mouse.mouseShown) {

            Vec3 radianRot = Vec3(glm::radians(camTransform.rotation.x), glm::radians(camTransform.rotation.y), glm::radians(camTransform.rotation.z));
            camera.rotation = Vec3(radianRot.x, radianRot.y, radianRot.z);
        }


        //Text render loop
        for (int i = 0; i < console.lines.size(); i++) {
            D3DGfx().ShowText(console.lines[i]);
        }
        //D3DGfx().ShowText(newText);



        PxVec3 pos;
        PxQuat rotQuat;
        PxVec3 rot;

        b2Vec2 pos2D;
        float rot2D;
        

        for (int i = 0; i < containers.size(); i++) {
            if (containers[i]->rigidbody.rbDynamic != nullptr) {
                pos = containers[i]->rigidbody.rbDynamic->getGlobalPose().p;
                rotQuat = containers[i]->rigidbody.rbDynamic->getGlobalPose().q;
                rot = rotQuat.getBasisVector1();

                containers[i]->transform.position = Vec3(pos.x, pos.y, pos.z);
                containers[i]->transform.rotation = Vec3(rot.x, rot.y, rot.z);
            }
            else if (containers[i]->rigidbody.rbStatic != nullptr) {
                pos = containers[i]->rigidbody.rbStatic->getGlobalPose().p;
                rotQuat = containers[i]->rigidbody.rbStatic->getGlobalPose().q;
                rot = rotQuat.getBasisVector1();

                containers[i]->transform.position = Vec3(pos.x, pos.y, pos.z);
                containers[i]->transform.rotation = Vec3(rot.x, rot.y, rot.z);
            }
            else if (containers[i]->rigidbody.rb2D != nullptr) {
                pos2D = containers[i]->rigidbody.rb2D->GetPosition();
                rot2D = containers[i]->rigidbody.rb2D->GetAngle();

                containers[i]->transform.position = Vec3(pos2D.x, pos2D.y, 0.0f);
                containers[i]->transform.rotation = Vec3(0.0f, 0.0f, rot2D);
            }
        }
        

        //Model render loop
        for (int i = 0; i < containers.size(); i++) {
            
            for (int j = 0; j < containers[i]->models.modelList.size(); j++) {
                D3DGfx().RenderModel(containers[i]->models.modelList[j], containers[i]->transform, camera, &modelImporter, hWnd);
            }
        }


        //Sprites Rendering
        for (int i = 0; i < containers.size(); i++) {

            for (int k = 0; k < containers[i]->sprites.sprites.size(); k++) {
                D3DGfx().DrawSprite(camera, Vec2(containers[i]->transform.position.x / 10.0f, containers[i]->transform.position.y / 10.0f), Vec2(1.0f, 1.0f), &containers[i]->sprites.sprites[k]->texture, hWnd);
            }
        }

        //Gfx().DrawTestCube(Color(10, 170, 170, 255), Vec3(pos.x, pos.y, pos.z), Rotation(rot.x, rot.y, rot.z), hWnd);
        ///D3DGfx().RenderModel(modelImporter.meshes[1], Vec3(pos2.x, pos2.y, pos2.z), Rotation(rot2.x, rot2.y, rot2.z), camera, hWnd); //Rotation(-mouse.mousePosition.y / 100, -mouse.mousePosition.x / 100,0.0f)
        //D3DGfx().DrawTestCube(Color(40, 170, 70, 255), Vec3(pos2.x, pos2.y, pos2.z), Rotation(rot2.x, rot2.y, rot2.z), camera, hWnd);

        ///D3DGfx().RenderModel(modelImporter.meshes[0], Vec3(pos.x, pos.y, pos.z), Rotation(rot.x, rot.y, rot.z), camera, hWnd); //Rotation(-mouse.mousePosition.y / 100, -mouse.mousePosition.x / 100,0.0f)

        //Gfx().RenderModel(modelImporter.meshes[0], Vec3(pos2.x, pos2.y, pos2.z), Rotation(rot2.x, rot2.y, rot2.z), hWnd); //Rotation(-mouse.mousePosition.y / 100, -mouse.mousePosition.x / 100,0.0f)



        //D3DGfx().DrawSprite(camera, Vec2(containers[2]->transform.position.x / 10.0f, containers[2]->transform.position.y / 10.0f), Vec2(1.0f, 1.0f), &sprite, hWnd);



        //Gfx().DrawRect(Vec2(0.0f, 0.0f), Vec2(1.0f, 1.0f), 0, 0 /*0.2f*/, Color(30, 30, 30, 255), hWnd);

        //Gfx().DrawTestCube(Color(255, 0, 0, 255), Rotation(rotTimer,rotTimer,rotTimer), hWnd);
        //Gfx().DrawTestCube(Color(255, 0, 0, 255), Rotation(rotTimer + 50,rotTimer + 50,rotTimer + 50), hWnd);

        D3DGfx().EndFrame();
    }
    else {
        physics.Simulate();
        physics.Simulate2D();


        //PxActor** actors;
        //physics.scene->getActors(PxActorTypeFlag::eRIGID_DYNAMIC,actors,1); //actors[0]->is<PxRigidDynamic>()->getGlobalPose().p.z


        


        //rot2Quat.toRadiansAndUnitAxis(angle2, rot2);
        //radians to degrees conversion (57.2958 degrees is 1 radian)
        /*rot2 *= angle2;
        rot2 /= 57.2958f;*/


        //FPS camera

        // Update rotations based on mouse delta
        camTransform.rotation.x += startEng.mouse.mouseDelta.y * sensitivity;
        camTransform.rotation.y += startEng.mouse.mouseDelta.x * sensitivity;

        // Clamp the rotationY to prevent flipping over at the poles
        camTransform.rotation.x = std::max(std::min(camTransform.rotation.x, 89.0f), -89.0f);

        camTransform.rotation = startEng.NormalizeAngles(camTransform.rotation);
        // Implement rotation around Z for VR
        // rotationZ += mouse.deltaZ * sensitivity; // Uncomment if you have deltaZ


        Vec3 radianRot = Vec3(glm::radians(camTransform.rotation.x), glm::radians(camTransform.rotation.y), glm::radians(camTransform.rotation.z));
        camera.rotation = Vec3(radianRot.x, radianRot.y, radianRot.z);

        /*char buffer[64];
        snprintf(buffer, sizeof(buffer), "%f", mouse.mouseDelta.x);

        glfwSetWindowTitle(window, buffer);*/


        GLGfx().ClearBuffer(0.2f, 0.2f, 0.7f);
        //GLGfx().DrawTestTri();
        //GLGfx().DrawTestCube(Color(40, 170, 70, 255), Vec3(pos.x, pos.y, pos.z), Rotation(rot.x, rot.y, rot.z), camera, window);
        //GLGfx().DrawTestCube(Color(40, 170, 70, 255), Vec3(pos2.x, pos2.y, pos2.z), Rotation(rot2.x, rot2.y, rot2.z), camera, window);
        
        
        
        PxVec3 pos;
        PxQuat rotQuat;
        PxVec3 rot;

        b2Vec2 pos2D;
        float rot2D;

        for (int i = 0; i < containers.size(); i++) {
            if (containers[i]->rigidbody.rbDynamic != nullptr) {
                pos = containers[i]->rigidbody.rbDynamic->getGlobalPose().p;
                rotQuat = containers[i]->rigidbody.rbDynamic->getGlobalPose().q;
                rot = rotQuat.getBasisVector1();

                containers[i]->transform.position = Vec3(pos.x, pos.y, pos.z);
                containers[i]->transform.rotation = Vec3(rot.x, rot.y, rot.z);
            }
            else if (containers[i]->rigidbody.rbStatic != nullptr) {
                pos = containers[i]->rigidbody.rbStatic->getGlobalPose().p;
                rotQuat = containers[i]->rigidbody.rbStatic->getGlobalPose().q;
                rot = rotQuat.getBasisVector1();

                containers[i]->transform.position = Vec3(pos.x, pos.y, pos.z);
                containers[i]->transform.rotation = Vec3(rot.x, rot.y, rot.z);
            }
            else if (containers[i]->rigidbody.rb2D != nullptr) {
                pos2D = containers[i]->rigidbody.rb2D->GetPosition();
                rot2D = containers[i]->rigidbody.rb2D->GetAngle();

                containers[i]->transform.position = Vec3(pos2D.x, pos2D.y, 0.0f);
                containers[i]->transform.rotation = Vec3(0.0f, 0.0f, rot2D);
            }
        }



        for (int i = 0; i < containers.size(); i++) {
            for (int j = 0; j < containers[i]->models.modelList.size(); j++) {
                GLGfx().RenderModel(containers[i]->models.modelList[j], containers[i]->transform, &camera, window);
            }
        }

        ///GLGfx().RenderModel(modelImporter.meshes[0], Vec3(pos.x, pos.y, pos.z), Rotation(rot.x, rot.y, rot.z), &camera, window);

        ///GLGfx().RenderModel(modelImporter.meshes[1], Vec3(pos2.x, pos2.y, pos2.z), Rotation(rot2.x, rot2.y, rot2.z), &camera, window); //cube

        // draw our first triangle
        glUseProgram(startEng.GLGfx().shaderProgram);
        glBindVertexArray(startEng.GLGfx().VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
        //glDrawArrays(GL_TRIANGLES, 0, 6);
        //glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0); //Called from individual draw functions
        // glBindVertexArray(0); // no need to unbind it every time 

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();


        //GLGfx().EndFrame();
    }
}

void StartEngine::InputThread() {

}



void StartEngine::SaveEngine() {
    std::string saveData;
    //store all model paths, transforms connected to those, sprites, rigidbodies, colliders, container properties/objects,
    
    //Format: '{}' for a container separation, each object in the containers are formatted by specific names:
    // 
    //{Transform((Position), (Rotation), (Scale)),
    // Rigidbody(isDynamic(t/f), hasCollision(t/f), gravity(float), is2D(t/f)),
    // Model(path(std::string)),
    // Sprite(path(std::string)),
    // Collider(is2D(t/f), inheritFromModel(t/f), vertices[(p1x,p1y,p1z), (p2x,p2y,p2z), ect.], indices[i1,i2,i3,ect.])}, 
    // {ect.}

    for (int i = 0; i < containers.size(); i++) {

        if (i != 0) {
            saveData += ",";
        }

        saveData += "{";

        //Transform
        saveData += "Transform(";
        //saveData += "[";
        saveData += std::to_string(containers[i]->transform.position.x);
        saveData += ",";
        saveData += std::to_string(containers[i]->transform.position.y);
        saveData += ",";
        saveData += std::to_string(containers[i]->transform.position.z);
        saveData += ",";
        //saveData += "],[";
        saveData += std::to_string(containers[i]->transform.rotation.x);
        saveData += ",";
        saveData += std::to_string(containers[i]->transform.rotation.y);
        saveData += ",";
        saveData += std::to_string(containers[i]->transform.rotation.z);
        saveData += ",";
        //saveData += "],[";
        saveData += std::to_string(containers[i]->transform.scale.x);
        saveData += ",";
        saveData += std::to_string(containers[i]->transform.scale.y);
        saveData += ",";
        saveData += std::to_string(containers[i]->transform.scale.z);
        saveData += ")";
        //saveData += "])";


        char dynamic;
        char is2D;

        if (containers[i]->rigidbody.isDynamic) {
            dynamic = 't';
        }
        else {
            dynamic = 'f';
        }

        if (containers[i]->rigidbody.rb2D != nullptr) {
            is2D = 't';
        }
        else {
            is2D = 'f';
        }

        saveData += ",Rigidbody(";
        saveData += dynamic;
        saveData += ",";
        saveData += is2D;
        saveData += ")";

        for (int j = 0; j < containers[i]->models.modelList.size(); j++) {
            Model* curModel = &containers[i]->models.modelList[j];

            if (curModel->modelPath != "") {
                saveData += ",Model(";
                saveData += "\"";
                saveData += curModel->modelPath;
                saveData += "\"";
                saveData += ")";
            }
            /*else {
                saveData += "[";

                for (int k = 0; k <= curModel->vertices.size(); k++) {
                    saveData += "(";

                    if (k != 0) {
                        saveData += ",";
                    }


                    saveData += "Color(";
                    saveData += std::to_string(curModel->vertices[k].color.r);
                    saveData += ",";
                    saveData += std::to_string(curModel->vertices[k].color.g);
                    saveData += ",";
                    saveData += std::to_string(curModel->vertices[k].color.b);
                    saveData += ",";
                    saveData += std::to_string(curModel->vertices[k].color.a);
                    saveData += ")";


                    saveData += ",Normal(";
                    saveData += std::to_string(curModel->vertices[k].normal.x);
                    saveData += ",";
                    saveData += std::to_string(curModel->vertices[k].normal.y);
                    saveData += ",";
                    saveData += std::to_string(curModel->vertices[k].normal.z);
                    saveData += ")";


                    saveData += ",Position(";
                    saveData += std::to_string(curModel->vertices[k].position.x);
                    saveData += ",";
                    saveData += std::to_string(curModel->vertices[k].position.y);
                    saveData += ",";
                    saveData += std::to_string(curModel->vertices[k].position.z);
                    saveData += ")";


                    saveData += ",UV(";
                    saveData += std::to_string(curModel->vertices[k].uv.x);
                    saveData += ",";
                    saveData += std::to_string(curModel->vertices[k].uv.y);
                    saveData += ")";


                    saveData += ")";
                }
                saveData += "]";

                saveData += ",[";

                for (int l = 0; l <= curModel->indices.size(); l++) {
                    if (l != 0) {
                        saveData += ",";
                    }

                    saveData += std::to_string(curModel->indices[l]);

                }
                saveData += "]";
            }*/
        }

        saveData += ",Name(\"";
        saveData += containers[i]->name;
        saveData += "\")";

        //Format: '{}' for a container separation, each object in the containers are formatted by specific names:
            // 
            // Model(path(std::string)),
            // Sprite(path(std::string)),
            // Collider(is2D(t/f), inheritFromModel(t/f), vertices[(p1x,p1y,p1z), (p2x,p2y,p2z), ect.], indices[i1,i2,i3,ect.])}, 
            // {ect.}
        saveData += "}";
    }

    WriteDataToFile(engineDataPath, saveData);
}

void StartEngine::LoadEngine() {
    std::string loadedData = ReadFromFile(engineDataPath);

    std::string loadedClass;
    std::string loadedSegment;
    int curlyBracket = 0;

    bool inClassSetup = false;
    int classParameters = 0;


    bool inSubClassSetup = false;
    int subClassParameters = 0;
    std::string loadedSubClass;


    bool inQuote = false;

    Container curContainer;

    for (int i = 0; i < loadedData.size(); i++) {

        char curChar = loadedData[i];

        if (curChar == '{' && !inQuote) {
            if (curlyBracket == 0) {
                curContainer = Container();
                //loadedContainers.emplace_back();
            }
            curlyBracket++;
            //create container and add the pointer to containers
        }

        if (curChar == '}' && !inQuote) {
            if (curlyBracket == 1) {
                loadedContainers.push_back(curContainer);
                containers.push_back(&loadedContainers.back());
            }
            curlyBracket--;
            //create container and add the pointer to containers
        }


        if (curlyBracket > 0) {
            loadedSegment += curChar;


            if (loadedSegment == "Name" && !inClassSetup && !inQuote) {
                loadedClass = loadedSegment;
            }

            if (loadedSegment == "Rigidbody" && !inClassSetup && !inQuote) {
                curContainer.rigidbody.physicsManager = &physics;
                loadedClass = loadedSegment;
            }

            if (loadedSegment == "Transform" && !inClassSetup && !inQuote) {
                loadedClass = loadedSegment;
            }

            if (loadedSegment == "Model" && !inClassSetup && !inQuote) {
                loadedClass = loadedSegment;
            }





            if (curChar == '\'' || curChar == '"') {
                //DisplayNumAsTitle(i);
                inQuote = !inQuote;
            }
            if (curChar == '(' && !inQuote) {
                if (inClassSetup) {
                    inSubClassSetup = true;
                }

                inClassSetup = true;
                loadedSegment = "";
                continue;
            }
            if (curChar == ')' && !inQuote) {
                if (inClassSetup && !inSubClassSetup) {
                    inClassSetup = false;
                    loadedClass = "";
                    classParameters = 0;
                }
                if (inSubClassSetup) {
                    inSubClassSetup = false;
                }
                loadedSegment = "";


                continue;
            }
            if (curChar == ',' && !inQuote) {
                if (inClassSetup) {
                    classParameters++;
                }
                loadedSegment = "";
                continue;
            }



            if (loadedClass == "Name" && !inQuote) {

                if (curChar == '\"' || curChar == '\'') {

                    curContainer.name = RemoveQuotes(loadedSegment);
                }
            }


            if (loadedClass == "Rigidbody" && inClassSetup && !inQuote) {

                if (classParameters == 0) {
                    if (loadedSegment == "t") {
                        curContainer.rigidbody.isDynamic = true;
                    }
                    else if (loadedSegment == "f") {
                        curContainer.rigidbody.isDynamic = false;
                    }
                }
                else if (classParameters == 1) {
                    if (loadedSegment == "t") {
                        curContainer.rigidbody.NewRB2D();
                    }
                    else if (loadedSegment == "f") {
                        curContainer.rigidbody.NewRB();
                    }
                }
            }


            if (loadedClass == "Transform" && inClassSetup && !inQuote) {

                char nextChar = loadedData[i + 1];

                if (classParameters == 0 && nextChar == ',') {
                    curContainer.transform.position.x = stof(loadedSegment);
                }
                else if (classParameters == 1 && nextChar == ',') {
                    curContainer.transform.position.y = stof(loadedSegment);
                }
                else if (classParameters == 2 && nextChar == ',') {
                    curContainer.transform.position.z = stof(loadedSegment);
                }

                else if (classParameters == 3 && nextChar == ',') {
                    curContainer.transform.rotation.x = stof(loadedSegment);
                }
                else if (classParameters == 4 && nextChar == ',') {
                    curContainer.transform.rotation.y = stof(loadedSegment);
                }
                else if (classParameters == 5 && nextChar == ',') {
                    curContainer.transform.rotation.z = stof(loadedSegment);
                }

                else if (classParameters == 6 && nextChar == ',') {
                    curContainer.transform.scale.x = stof(loadedSegment);
                }
                else if (classParameters == 7 && nextChar == ',') {
                    curContainer.transform.scale.y = stof(loadedSegment);
                }
                else if (classParameters == 8 && nextChar == ',') {
                    curContainer.transform.scale.z = stof(loadedSegment);
                }
            }


            if (loadedClass == "Model" && !inQuote) {

                if (curChar == '\"' || curChar == '\'') {

                    //DisplayStringAsTitle(RemoveQuotes(loadedSegment));
                    //DisplayNumAsTitle(i);
                    std::string modelPath = RemoveQuotes(loadedSegment);
                    //DisplayStringAsTitle((char*)modelPath.c_str());
                    modelImporter.ImportModel((char*)modelPath.c_str(), &texLoader);

                    curContainer.models.modelList.push_back(modelImporter.meshes.back());
                    curContainer.models.modelList.back().modelPath = modelPath;
                    D3DGfx().RenderModel(curContainer.models.modelList.back(), curContainer.transform, camera, &modelImporter, hWnd);
                }
                //loadedSegment
            }



            //Format: '{}' for a container separation, each object in the containers are formatted by specific names:
            // 
            //{Transform((Position), (Rotation), (Scale)),
            // Rigidbody(isDynamic(t/f), hasCollision(t/f), gravity(float), is2D(t/f)),
            // Model(path(std::string)),
            // Sprite(path(std::string)),
            // Collider(is2D(t/f), inheritFromModel(t/f), vertices[(p1x,p1y,p1z), (p2x,p2y,p2z), ect.], indices[i1,i2,i3,ect.])}, 
            // {ect.}
        }
    }
    //DisplayNumAsTitle(containers.size());
    std::string newTitle;
    newTitle += containers[0]->transform.position.x;
    newTitle += ", ";
    newTitle += containers[0]->transform.position.y;
    newTitle += ", ";
    newTitle += containers[0]->transform.position.z;
}

//void StartEngine::LoadEngine() {
//    std::string loadedData = ReadFromFile(engineDataPath);
//
//    std::istringstream iss(loadedData);
//    std::string token;
//
//    // Iterate over comma-separated object descriptions
//    while (std::getline(iss, token, ',')) {
//        std::istringstream obj_stream(token);
//        std::string segment;
//
//        Container container;
//
//        // Iterate over segments within each object description
//        while (std::getline(obj_stream, segment, ',')) {
//            // Check for each segment type and parse accordingly
//            // Transform segment
//            if (segment.find("Transform") != std::string::npos) {
//                std::istringstream transform_stream(segment);
//                char tmp;
//                transform_stream >> tmp; // Consume 'Transform'
//                transform_stream >> tmp; // Consume '('
//                transform_stream >> container.transform.position.x;
//                transform_stream >> tmp; // Consume ','
//                transform_stream >> container.transform.position.y;
//                transform_stream >> tmp; // Consume ','
//                transform_stream >> container.transform.position.z;
//                // Repeat for rotation and scale
//                transform_stream >> tmp; // Consume ','
//                transform_stream >> container.transform.rotation.x;
//                transform_stream >> tmp; // Consume ','
//                transform_stream >> container.transform.rotation.y;
//                transform_stream >> tmp; // Consume ','
//                transform_stream >> container.transform.rotation.z;
//                transform_stream >> tmp; // Consume ','
//                transform_stream >> container.transform.scale.x;
//                transform_stream >> tmp; // Consume ','
//                transform_stream >> container.transform.scale.y;
//                transform_stream >> tmp; // Consume ','
//                transform_stream >> container.transform.scale.z;
//            }
//            // Rigidbody segment
//            else if (segment.find("Rigidbody") != std::string::npos) {
//                std::istringstream rigidbody_stream(segment);
//                char tmp;
//                rigidbody_stream >> tmp; // Consume 'Rigidbody'
//                rigidbody_stream >> tmp; // Consume '('
//                std::string isDynamicStr;
//                rigidbody_stream >> isDynamicStr; // Read isDynamic
//                container.rigidbody.physicsManager = &physics;
//                container.rigidbody.isDynamic = (isDynamicStr == "t");
//                rigidbody_stream >> tmp; // Consume ','
//                std::string is2DStr;
//                rigidbody_stream >> is2DStr; // Read is2D
//                if (is2DStr == "t") {
//
//                    container.rigidbody.NewRB2D();
//                }
//                else {
//
//                    container.rigidbody.NewRB();
//                }
//            }
//            // Model segment
//            else if (segment.find("Model") != std::string::npos) {
//                std::istringstream model_stream(segment);
//                char tmp;
//                model_stream >> tmp; // Consume 'Model'
//                model_stream >> tmp; // Consume '('
//                std::getline(model_stream, container.models.modelList[0].modelPath, '\"');
//            }
//            // Name segment
//            else if (segment.find("Name") != std::string::npos) {
//                std::istringstream name_stream(segment);
//                char tmp;
//                name_stream >> tmp; // Consume 'Name'
//                name_stream >> tmp; // Consume '('
//                std::getline(name_stream, container.name, '\"');
//            }
//        }
//
//        // Add populated container to the vector
//        containers.push_back(&container);
//    }
//}

std::string RemoveQuotes(std::string& inString) {
    int strLen = inString.length();
    if (strLen >= 3) {
        return inString.substr(1, inString.length() - 2);
    }
    else {
        return inString;
    }
}

D3DGraphics& StartEngine::D3DGfx() {
     return *postD3DGraphics;
}

GLGraphics& StartEngine::GLGfx() {
    return *postGLGraphics;
}


FontCreation& StartEngine::Font() {
    return *fontManagement;
}



Vec3 StartEngine::NormalizeAngles(Vec3 input) {
    return Vec3(fmod(input.x, 360.0f), fmod(input.y, 360.0f), fmod(input.z, 360.0f));
}


//Setup Break Point:

//   \/  \/  \/  \/  \/  \/  \/  \/   Registers the window class.    WNDCLASSEX
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEX wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_VREDRAW | CS_HREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(APP_LOGO)); /*Need app icon*/
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = MAKEINTRESOURCE(APP_NAME);
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(APP_LOGO_SMALL));


    return RegisterClassEx(&wcex);
}

//Creates actual window
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow, Vec2 windSize)
{
    hInst = hInstance; // Store instance handle in our global variable

    RECT screenRect;
    HWND screenSize = GetDesktopWindow();
    GetWindowRect(screenSize,&screenRect);


    if (isD3D) {
        hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW, //WS_POPUP, WS_OVERLAPPEDWINDOW
            (screenRect.right / 2) - (windSize.x / 2), (screenRect.bottom / 2) - (windSize.y / 2), windSize.x, windSize.y, nullptr, nullptr, hInstance, nullptr);


        if (!hWnd)
        {
            return FALSE;
        }
        

        ShowWindow(hWnd, nCmdShow);
        UpdateWindow(hWnd);

        startEng.postD3DGraphics = std::make_unique<D3DGraphics>(hWnd);
    }
    else {
        if (!glfwInit()) {
            return FALSE;
        }
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_DEPTH_BITS, 24); //Depth
        //glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
        //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

        size_t sizeNeeded;
        wcstombs_s(&sizeNeeded, nullptr, 0, szTitle, 0);

        // Allocate the multibyte string buffer
        char* mbstr = new char[sizeNeeded];

        // Convert the wide character string to a multibyte string
        size_t convertedChars;
        errno_t charString = wcstombs_s(&convertedChars, mbstr, sizeNeeded, szTitle, _TRUNCATE);


        window = glfwCreateWindow(windSize.x, windSize.y, mbstr, NULL, NULL);
        delete[] mbstr;

        if (window == NULL)
        {
            std::cout << "Failed to create GLFW window" << std::endl;
            glfwTerminate();
            return FALSE;
        }
        glfwMakeContextCurrent(window);

        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            std::cout << "Failed to initialize GLAD" << std::endl;
            return FALSE;
        }

        glViewport(0, 0, windSize.x, windSize.y);

        //Graphics initialization
        startEng.postGLGraphics = std::make_unique<GLGraphics>();
        /*startEng.GLGfx().SetKeyboard(&startEng.GetKeyboard());
        startEng.GLGfx().SetMouse(&startEng.GetMouse());*/

        

        glfwSetFramebufferSizeCallback(window, startEng.GLGfx().frameBufferCallback);
        glfwSetWindowCloseCallback(window, startEng.GLGfx().windowCloseCallback);

        startEng.input.mouse = &startEng.GetMouse();
        startEng.input.keyboard = &startEng.GetKeyboard();


        //Mouse Section:
        glfwSetWindowUserPointer(window, &startEng.input);

        glfwSetMouseButtonCallback(window, startEng.GLGfx().mouseClickCallback);
        glfwSetCursorPosCallback(window, startEng.GLGfx().mouseMoveCallback);
        glfwSetScrollCallback(window, startEng.GLGfx().scrollCallback);
        
        
        //Keyboard Section:
        glfwSetWindowUserPointer(window, &startEng.input);
        
        glfwSetKeyCallback(window, startEng.GLGfx().keyCallback);

    }



    return TRUE;
}




//GL callbacks:






//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_COMMAND:
    {
        if (isD3D) {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
            case IDM_ABOUT:
                //DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, msg, wParam, lParam);
            }
        }
    }
    break;


    case Msgbox:
    {
        std::ostringstream newStr;
        newStr << "L Param: " << lParam << ", W Param: " << wParam;
        
        MessageBoxA(hWnd, newStr.str().c_str(), "", 0);
        SetWindowTextA(hWnd, newStr.str().c_str());
    }
    break;


    case WM_PAINT:
    {
        if (isD3D) {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: Add any drawing code that uses hdc here...

            EndPaint(hWnd, &ps);
        }
    }
    break;

    case WM_DESTROY:
    {
        if (isD3D) {
            exited = true;
            PostQuitMessage(0);
        }
    }
    break;

    case WM_KILLFOCUS:
    {
        startEng.keyboard.ClearKeyStates();
        startEng.mouse.ClearStates();

        startEng.mouse.UpdateMouse(); //updates scroll wheel delta,   MAYBE: add a mouse movement delta, to add support for mouse movement per frame
        startEng.mouse.D3DShowMouse();
        startEng.mouse.mouseShown = true;
        //reset all key states in app
    }
    break;

    case WM_KEYDOWN:
    {
        // To Do:
        // * move keyboard events to a separate class
        // * returns true if 1, it's not a true onDown(); 
        // * Use WM_SYSKEYDOWN and WM_SYSKEYUP for ctrl, alt, window key, ext, Not Visual Keys; same logic.





        /*static WindowsMessageMap mm;
        OutputDebugString((LPCWSTR)mm(msg, lParam, wParam).c_str());
        
        Use: TEXT("Example Text"),   to convert from const char* (string) to LPCWCHAR or a string of text
        
        */

        startEng.keyboard.OnKeyDown(wParam);
    }
    break;

    //WM_CHAR for text
    case WM_KEYUP:
    {
        startEng.keyboard.OnKeyUp(wParam);
    }
    break;
    
    case WM_CHAR:
    {
        startEng.keyboard.OnChar(wParam);
    }
    break;


    case WM_SYSKEYDOWN:
    {
        startEng.keyboard.OnKeyDown(wParam);
    }
    break;

    case WM_SYSKEYUP:
    {
        startEng.keyboard.OnKeyUp(wParam);
    }
    break;



    case WM_MOUSEMOVE:
    {
        POINTS pts = MAKEPOINTS(lParam);



        RECT rect;

        GetWindowRect(hWnd, &rect);


        int width = (rect.right - rect.left);
        int height = (rect.bottom - rect.top);


        if (pts.x >= 0 && pts.x < width && pts.y >= 0 && pts.y < height) {

            startEng.mouse.OnMouseMove(pts.x,pts.y, startEng.mouse.mouseShown, startEng.mouse.D3DGetCenter(hWnd));

            //mouse in window
            if (!startEng.mouse.inWindow) {

                SetCapture(hWnd);

                startEng.mouse.OnMouseEnter();
            }
        }
        else {
            if ( startEng.mouse.Button0 & (MK_LBUTTON | MK_RBUTTON)) {

                startEng.mouse.OnMouseMove(pts.x, pts.y, startEng.mouse.mouseShown, startEng.mouse.D3DGetCenter(hWnd));
            }
            else {
                ReleaseCapture();
                startEng.mouse.OnMouseLeave();
                //mouse leave
            }
        }
    }
    break;



    case WM_MOUSEWHEEL:
    {
        if (GET_WHEEL_DELTA_WPARAM(wParam) > 0) {
            startEng.mouse.OnWheelUp();
        }
        else if (GET_WHEEL_DELTA_WPARAM(wParam) < 0) {
            startEng.mouse.OnWheelDown();
        }
    }
    break;






    case WM_LBUTTONDOWN: //LButton
    {
        // NOTES:
        // * Capture mouse call to get mouse pos off screen relative to the borders

        startEng.mouse.OnButton0Down();
    }
    break;

    case WM_LBUTTONUP: //LButton
    {
        // NOTES:
        // * Capture mouse call to get mouse pos off screen relative to the borders
        //
        // * Unrelated: structs can be used to create different variable types and structures like dictionaries or lists,
        // struct Vector3(){
        // std::float x,
        // std::float y,
        // std::float z
        //}

        startEng.mouse.OnButton0Up();
    }
    break;


    case WM_RBUTTONDOWN: //RButton
    {
        startEng.mouse.OnButton1Down();
    }
    break;

    case WM_RBUTTONUP: //RButton
    {
        startEng.mouse.OnButton1Up();
    }
    break;







    default:
        return DefWindowProc(hWnd, msg, wParam, lParam);
    }

    
    return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}