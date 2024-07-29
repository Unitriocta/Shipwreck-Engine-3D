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

using namespace GameVariables;

#include <dxtex/DirectXTex.h>







#include "testing.h"


namespace EngineInstance {

    StartEngine startEng;

    Input input;

    NetworkManager networkManager;

    Container cameraContainer;
    Camera* camera = &cameraContainer.camera;

    ModelImporter modelImporter;

    D3DTexture texLoader;

    Physics physics;

    HWND hWnd;

    int windowState = 0; //0: Windowed, 1: Maximized, 2: Fullscreen  //maybe change to 2 later
    float windowWidth;
    float windowHeight;

    float defaultWindowWidth = 900; //set to monitor size
    float defaultWindowHeight = 900;

    D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_0;

    GLFWwindow* window;

    bool exitedProgram = false;

    bool isD3D = true;

    std::mutex renderMutex;



    void DisplayNumAsTitle(float newVar) {

        std::ostringstream oss;
        oss << "variable: " << newVar;


        std::string widthStr = oss.str();

        //MessageBoxA(hWnd, widthStr.c_str(), "Hey", 0);
        SetWindowTextA(hWnd, widthStr.c_str());
    }

    void DisplayNumIntAsTitle(int newVar) {

        std::ostringstream oss;
        oss << "variable: " << newVar;


        std::string widthStr = oss.str();

        //MessageBoxA(hWnd, widthStr.c_str(), "Hey", 0);
        SetWindowTextA(hWnd, widthStr.c_str());
    }


    struct MonitorInfo {
        RECT monitorRect; // Coordinates of the monitor in virtual screen coordinates
        RECT workRect;    // Coordinates of the monitor's work area (excluding taskbar)
    };

    BOOL CALLBACK MonitorEnumProc(HMONITOR hMonitor, HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData) {
        std::vector<MonitorInfo>* monitorInfoList = reinterpret_cast<std::vector<MonitorInfo>*>(dwData);

        MONITORINFOEX monitorInfo;
        monitorInfo.cbSize = sizeof(MONITORINFOEX);
        if (GetMonitorInfo(hMonitor, &monitorInfo)) {
            MonitorInfo info;
            info.monitorRect = monitorInfo.rcMonitor;
            info.workRect = monitorInfo.rcWork;
            monitorInfoList->push_back(info);
        }
        return TRUE;
    }

    std::vector<MonitorInfo> GetMonitorsInfo() {
        std::vector<MonitorInfo> monitorInfoList;
        EnumDisplayMonitors(nullptr, nullptr, MonitorEnumProc, reinterpret_cast<LPARAM>(&monitorInfoList));
        return monitorInfoList;
    }

    void ToggleFullscreen() {

        std::vector<MonitorInfo> monitors = GetMonitorsInfo();
        for (size_t i = 0; i < monitors.size(); i++) {
            MonitorInfo& info = monitors[i];
        }


        BOOL fullscreen;
        startEng.D3DGfx().swapChain->GetFullscreenState(&fullscreen, nullptr);
        if (fullscreen) {
            windowState = 0; //or 1, we'll add a bool later to check if

            windowWidth = defaultWindowWidth;
            windowHeight = defaultWindowHeight;

            //startEng.D3DGfx().swapChain->SetFullscreenState(!fullscreen, nullptr);
            //SetWindowLongPtr(hWnd, GWL_STYLE, WS_OVERLAPPEDWINDOW);
            //SetWindowPos(hWnd, HWND_TOP, (monitors[0].workRect.right - monitors[0].workRect.left) / 2, (monitors[0].workRect.bottom - monitors[0].workRect.top) / 2, defaultWindowWidth, defaultWindowHeight, SWP_FRAMECHANGED | SWP_SHOWWINDOW);
            //windowWidth = defaultWindowWidth;
            //windowHeight = defaultWindowHeight;
        }
        else {
            windowState = 2; //or 1, we'll add a bool later to check if

            //startEng.D3DGfx().swapChain->SetFullscreenState(!fullscreen, nullptr);
            //SetWindowLongPtr(hWnd, GWL_STYLE, WS_POPUP);
            //windowWidth = monitors[0].monitorRect.right - monitors[0].monitorRect.left;
            //windowHeight = monitors[0].monitorRect.bottom - monitors[0].monitorRect.top;

            windowWidth = monitors[0].monitorRect.right - monitors[0].monitorRect.left;
            windowHeight = monitors[0].monitorRect.bottom - monitors[0].monitorRect.top;
            //SetWindowPos(hWnd, HWND_TOP, 0, 0, windowWidth, windowHeight, SWP_FRAMECHANGED | SWP_SHOWWINDOW);
        }

        startEng.D3DGfx().RefreshWindow(hWnd);
    }
}



#define MAX_LOADSTRING 100

HINSTANCE hInst;
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];

ATOM         MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE hInstance, int nCmdShow, Vec2 windSize);
LRESULT CALLBACK      WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);



FontCreation::SetupFace *setupFace;

Textbox newText;

//Transform camTransform;

Button button;

std::vector<Rigidbody> loadedRigidbodies;
std::vector<Model> loadedModels;
std::vector<Container> loadedContainers;




int targetFPS = 240;
std::chrono::milliseconds targetFrameLength(1000 / targetFPS);

const std::string engineDataPath = "D:/Shipwreck Engine 3D/Shipwreck Engine 3D/data.json";


void UpdateWindowSize();


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
    if (!InitInstance(hInstance, nCmdShow, Vec2(defaultWindowWidth, defaultWindowHeight)))
    {
        return FALSE;
    }

    if (!startEng.SetupXRInput()) {
        exit(5325);
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


    input.mouse = &startEng.mouse;
    input.keyboard = &startEng.keyboard;
    input.xrInput = &startEng.xrInput;


    for (int i = 0; i < startupScripts.size(); i++) {
        startupScripts[i]->Start(nullptr);
    }


    //Scripts Added to containers
    for (int i = 0; i < startEng.containers.size(); i++) {
        for (int j = 0; j < startEng.containers[i]->scriptNames.size(); j++) {
            if (scriptFactories[startEng.containers[i]->scriptNames[j]] != nullptr) {
                AddScriptToGameObject(startEng.containers[i], startEng.containers[i]->scriptNames[j]);
            }
        }
    }


    for (int i = 0; i < startEng.containers.size(); i++) {
        for (int j = 0; j < startEng.containers[i]->scripts.size(); j++) {

            startEng.containers[i]->scripts[j]->Start(startEng.containers[i]);
        }
    }
    

    //{Transform(0.000000,-4.000000,5.000000,0.025306,0.974694,-0.222105,0.000000,0.000000,0.000000),Rigidbody(f,f),Model("C:/Users/smsal/OneDrive/Documents/Blender Modules/cube.fbx"),Name("Ground")},{Transform(0.000000,16.000000,5.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000),Rigidbody(t,f),Model("C:/Users/smsal/OneDrive/Documents/Blender Modules/sphereV2.fbx"),Name("Ball")},{Transform(0.000000,9.997253,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000),Rigidbody(t,t),Name("PlayerSprite")}
    //startEng.LoadEngine();
    //startEng.SaveEngine();
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


    MSG msg = MSG();

    /*std::thread renderThread(&StartEngine::RenderFrame, &startEng);
    std::thread inputThread(&StartEngine::HandleInput, &startEng);
    
    renderThread.join();
    inputThread.join();*/

    // Main message loop:
    while (!exitedProgram && !glfwWindowShouldClose(window))
    {
        auto frameStart = std::chrono::high_resolution_clock::now();
        /*
        if (GetMessage(&msg, nullptr, 0, 0) <= 0) {

        }*/
        if (isD3D) {

            while (PeekMessage(&msg, hWnd, 0, 0, PM_REMOVE)) {

                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
        if (!isD3D) {
            glfwPollEvents();
        }

        startEng.ProcessXRInput();



        //startEng.networkManager.StartRecieveThread(startEng.networkManager);
        UpdateWindowSize();
        
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

                //startEng.networkManager.client.StartSendThread((char*)"SetPlayerPosition:7:5:7:0:");
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

    std::lock_guard<std::mutex> guard(renderMutex);
    //std::lock_guard<std::mutex> guard(playerMutex);



    /*camera.transform.update();
    camera.transform.updateGlobalProperties();


    for (int i = 0; i < containers.size(); i++) {
        containers[i]->transform.updateQuaternion();
        containers[i]->transform.update();
    }*/
    camera->transform.update();
    camera->transform.updateGlobalProperties();
    for (int i = 0; i < containers.size(); i++) {
        containers[i]->transform.updateQuaternion();
        containers[i]->transform.update();
    }

    for (int i = 0; i < containers.size(); i++) {
        for (int j = 0; j < containers[i]->scripts.size(); j++) {
            containers[i]->scripts[j]->Update(timeManager.GetTime(), containers[i]);
        }
    }


    if (isD3D) {





        physics.Simulate();
        physics.Simulate2D();


        PxVec3 pos;
        PxQuat rotQuat;
        glm::vec3 rot;

        b2Vec2 pos2D;
        float rot2D;

        
        //Physics rotations:
        for (int i = 0; i < containers.size(); i++) {
            if (containers[i]->rigidbody.rbDynamic != nullptr) {
                pos = containers[i]->rigidbody.rbDynamic->getGlobalPose().p;
                rotQuat = containers[i]->rigidbody.rbDynamic->getGlobalPose().q;
                rot = glm::eulerAngles(glm::quat(rotQuat.w, rotQuat.x, rotQuat.y, rotQuat.z));

                containers[i]->transform.position = Vec3(pos.x, pos.y, pos.z);
                containers[i]->transform.rotation = Vec3(glm::degrees(rot.x), glm::degrees(rot.y), glm::degrees(rot.z));
            }
            else if (containers[i]->rigidbody.rbStatic != nullptr) {
                pos = containers[i]->rigidbody.rbStatic->getGlobalPose().p;
                rotQuat = containers[i]->rigidbody.rbStatic->getGlobalPose().q;
                rot = glm::eulerAngles(glm::quat(rotQuat.w, rotQuat.x, rotQuat.y, rotQuat.z));

                containers[i]->transform.position = Vec3(pos.x, pos.y, pos.z);
                containers[i]->transform.rotation = Vec3(glm::degrees(rot.x), glm::degrees(rot.y), glm::degrees(rot.z));
            }
            else if (containers[i]->rigidbody.rb2D != nullptr) {
                pos2D = containers[i]->rigidbody.rb2D->GetPosition();
                rot2D = glm::degrees(containers[i]->rigidbody.rb2D->GetAngle());

                containers[i]->transform.position = Vec3(pos2D.x, pos2D.y, -5.0f);
                containers[i]->transform.rotation = Vec3(0.0f, 0.0f, rot2D);
            }
        }


        for (int i = 0; i < startEng.containers.size(); i++) {
            startEng.containers[i]->transform.updateGlobalProperties();
        }







        XrEventDataBuffer eventData = { XR_TYPE_EVENT_DATA_BUFFER };


        bool canRenderXR = false;

        while (xrPollEvent(xrInstance, &eventData) == XR_SUCCESS) {
            if (eventData.type == XR_TYPE_EVENT_DATA_SESSION_STATE_CHANGED) {
                XrEventDataSessionStateChanged* sessionStateChanged =
                    reinterpret_cast<XrEventDataSessionStateChanged*>(&eventData);

                // Output or handle the session state
                XrSessionState state = sessionStateChanged->state;
                switch (state) {
                case XR_SESSION_STATE_IDLE:
                    break;
                case XR_SESSION_STATE_READY:
                    canRenderXR = true;
                    break;
                case XR_SESSION_STATE_SYNCHRONIZED:
                    // Handle the synchronized state
                    break;
                case XR_SESSION_STATE_VISIBLE:
                    // Handle the visible state
                    break;
                case XR_SESSION_STATE_FOCUSED:
                    // Handle the focused state
                    break;
                case XR_SESSION_STATE_STOPPING:
                    // Handle the stopping state
                    break;
                case XR_SESSION_STATE_EXITING:
                    // Handle the exiting state
                    break;
                case XR_SESSION_STATE_LOSS_PENDING:
                    // Handle the loss pending state
                    break;
                default:
                    break;
                }
            }
        }





        if (canRenderXR) {



            //OpenXR
            XrFrameBeginInfo frameBeginInfo = {};
            frameBeginInfo.type = XR_TYPE_FRAME_BEGIN_INFO;
            xrBeginFrame(xrSession, &frameBeginInfo);


            uint32_t imageIndex;

            XrSwapchainImageAcquireInfo leftEyeAcquireInfo = { XR_TYPE_SWAPCHAIN_IMAGE_ACQUIRE_INFO };
            XrSwapchainImageWaitInfo leftEyeWaitInfo = { XR_TYPE_SWAPCHAIN_IMAGE_WAIT_INFO, 0, XR_INFINITE_DURATION };

            xrAcquireSwapchainImage(xrLeftEyeSwapchain, &leftEyeAcquireInfo, &imageIndex);
            xrWaitSwapchainImage(xrLeftEyeSwapchain, &leftEyeWaitInfo);



            uint32_t swapchainImageCount;
            xrEnumerateSwapchainImages(xrLeftEyeSwapchain, 0, &swapchainImageCount, nullptr);

            std::vector<XrSwapchainImageD3D11KHR> swapchainImageData(swapchainImageCount, { XR_TYPE_SWAPCHAIN_IMAGE_D3D11_KHR });

            xrEnumerateSwapchainImages(xrLeftEyeSwapchain, swapchainImageCount, &swapchainImageCount, reinterpret_cast<XrSwapchainImageBaseHeader*>(swapchainImageData.data()));






            ID3D11Texture2D* leftEyeTexture = swapchainImageData[imageIndex].texture;
            leftEyeRTV = nullptr;
            D3D11_RENDER_TARGET_VIEW_DESC rtvDesc = {};
            rtvDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
            rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
            rtvDesc.Texture2D.MipSlice = 0;
            HRESULT hResult = D3DGfx().device->CreateRenderTargetView(leftEyeTexture, &rtvDesc, &leftEyeRTV);
            if (FAILED(hResult)) {

                throw std::runtime_error("w");
            }




            //Right
            XrSwapchainImageAcquireInfo rightEyeAcquireInfo = { XR_TYPE_SWAPCHAIN_IMAGE_ACQUIRE_INFO };
            XrSwapchainImageWaitInfo rightEyeWaitInfo = { XR_TYPE_SWAPCHAIN_IMAGE_WAIT_INFO, 0, XR_INFINITE_DURATION };

            xrAcquireSwapchainImage(xrRightEyeSwapchain, &rightEyeAcquireInfo, &imageIndex);
            xrWaitSwapchainImage(xrRightEyeSwapchain, &rightEyeWaitInfo);


            ID3D11Texture2D* rightEyeTexture = swapchainImageData[imageIndex].texture;
            rightEyeRTV = nullptr;
            rtvDesc = {};
            rtvDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
            rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
            rtvDesc.Texture2D.MipSlice = 0;
            hResult = D3DGfx().device->CreateRenderTargetView(rightEyeTexture, &rtvDesc, &rightEyeRTV);
            if (FAILED(hResult)) {

                throw std::runtime_error("w");
            }




            ID3D11RenderTargetView* rtvs[3];
            rtvs[0] = leftEyeRTV;
            rtvs[1] = rightEyeRTV;
            rtvs[2] = D3DGfx().target;

            D3DGfx().deviceContext->OMSetRenderTargets(3, rtvs, D3DGfx().depthView);

            startEng.D3DGfx().ClearBuffer(0.2f, 0.2f, 0.7f, leftEyeRTV);
            startEng.D3DGfx().ClearBuffer(0.2f, 0.2f, 0.7f, rightEyeRTV);
            startEng.D3DGfx().ClearBuffer(0.2f, 0.2f, 0.7f, D3DGfx().target);
            startEng.D3DGfx().ClearDepth();

            RenderScene();


            XrSwapchainImageReleaseInfo releaseInfo = { XR_TYPE_SWAPCHAIN_IMAGE_RELEASE_INFO };
            xrReleaseSwapchainImage(xrLeftEyeSwapchain, &releaseInfo);

            xrReleaseSwapchainImage(xrRightEyeSwapchain, &releaseInfo);



            XrTime predictedDisplayTime = GetPredictedDisplayTime();



            XrCompositionLayerBaseHeader* layers[] = { reinterpret_cast<XrCompositionLayerBaseHeader*>(&projectionLayer) };


            XrFrameEndInfo frameEndInfo = {};
            frameEndInfo.type = XR_TYPE_FRAME_END_INFO;
            frameEndInfo.displayTime = predictedDisplayTime;
            frameEndInfo.environmentBlendMode = XR_ENVIRONMENT_BLEND_MODE_OPAQUE;
            frameEndInfo.layerCount = 1;
            frameEndInfo.layers = layers;

            xrEndFrame(xrSession, &frameEndInfo);






            //Comment this maybe to test vr, nvm
            D3DGfx().EndFrame();
        }
        else {

            D3DGfx().deviceContext->OMSetRenderTargets(1, &D3DGfx().target, D3DGfx().depthView);

            startEng.D3DGfx().ClearBuffer(0.2f, 0.2f, 0.7f, D3DGfx().target);
            startEng.D3DGfx().ClearDepth();

            RenderScene();



            //Comment this maybe to test vr
            D3DGfx().EndFrame();
        }
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
        //camTransform.rotation.x += startEng.mouse.mouseDelta.y * sensitivity;
        //camTransform.rotation.y += startEng.mouse.mouseDelta.x * sensitivity;

        //// Clamp the rotationY to prevent flipping over at the poles
        //camTransform.rotation.x = std::max(std::min(camTransform.rotation.x, 89.0f), -89.0f);

        //camTransform.rotation = startEng.NormalizeAngles(camTransform.rotation);
        //// Implement rotation around Z for VR
        //// rotationZ += mouse.deltaZ * sensitivity; // Uncomment if you have deltaZ


        //Vec3 radianRot = Vec3(glm::radians(camTransform.rotation.x), glm::radians(camTransform.rotation.y), glm::radians(camTransform.rotation.z));
        //camera.transform.rotation = Vec3(radianRot.x, radianRot.y, radianRot.z);

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
                GLGfx().RenderModel(containers[i]->models.modelList[j], containers[i]->transform, camera, window);
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



void StartEngine::RenderScene() {

    for (int i = 0; i < containers.size(); i++) {
        for (int j = 0; j < containers[i]->models.modelList.size(); j++) {
            D3DGfx().RenderModel(containers[i]->models.modelList[j], containers[i]->transform, *camera, hWnd);
        }
    }

    for (int i = 0; i < containers.size(); i++) {
        for (int j = 0; j < containers[i]->models.skinnedModelList.size(); j++) {
            D3DGfx().RenderSkinnedModel(containers[i]->models.skinnedModelList[j], containers[i]->transform, *camera, hWnd);
        }
    }

    //Sprites
    for (int i = 0; i < containers.size(); i++) {
        if (containers[i]->sprites.sprites.size() > 0) {
            for (int k = 0; k < containers[i]->sprites.sprites.size(); k++) {
                D3DGfx().DrawSprite(*camera,
                    Vec2(containers[i]->transform.globalPosition.x / 10.0f, containers[i]->transform.globalPosition.y / 10.0f),
                    Vec2(1.0f, 1.0f),
                    &containers[i]->sprites.sprites[k]->texture,
                    hWnd);
            }
        }
    }
}




void StartEngine::InputThread() {

}



XrTime StartEngine::GetPredictedDisplayTime() {

    XrFrameState frameState = {};
    frameState.type = XR_TYPE_FRAME_STATE;

    XrResult result = xrWaitFrame(xrSession, nullptr, &frameState);
    if (XR_FAILED(result)) {
        return 0;
    }

    return frameState.predictedDisplayTime;
}



void StartEngine::SetupXRLayers() {



    XrReferenceSpaceCreateInfo referenceSpaceCreateInfo = {};
    referenceSpaceCreateInfo.type = XR_TYPE_REFERENCE_SPACE_CREATE_INFO;
    referenceSpaceCreateInfo.referenceSpaceType = XR_REFERENCE_SPACE_TYPE_STAGE; // or XR_REFERENCE_SPACE_TYPE_LOCAL
    referenceSpaceCreateInfo.poseInReferenceSpace.orientation.w = 1.0f;
    referenceSpaceCreateInfo.poseInReferenceSpace.position = { 0.0f, 0.0f, 0.0f };

    XrSpace referenceSpace;
    XrResult result = xrCreateReferenceSpace(xrSession, &referenceSpaceCreateInfo, &referenceSpace);
    if (XR_FAILED(result)) {

    }



    projectionLayer = {};
    projectionLayer.type = XR_TYPE_COMPOSITION_LAYER_PROJECTION;
    projectionLayer.layerFlags = 0;
    projectionLayer.space = referenceSpace;



    float horizontalFovDegrees = 100.0f;
    float verticalFovDegrees = 100.0f;

    XrFovf fov = {};
    fov.angleLeft = glm::radians(-horizontalFovDegrees * 0.5f);
    fov.angleRight = glm::radians(horizontalFovDegrees * 0.5f);
    fov.angleUp = glm::radians(verticalFovDegrees * 0.5f);
    fov.angleDown = glm::radians(-verticalFovDegrees * 0.5f);




    
    XrPosef defaultPose = {};
    defaultPose.orientation.x = 0.0f;
    defaultPose.orientation.y = 0.0f;
    defaultPose.orientation.z = 0.0f;
    defaultPose.orientation.w = 1.0f;
    defaultPose.position.x = 0.0f;
    defaultPose.position.y = 0.0f;
    defaultPose.position.z = 0.0f;




    projectionViews[0].type = XR_TYPE_COMPOSITION_LAYER_PROJECTION_VIEW;
    projectionViews[0].pose = defaultPose;
    projectionViews[0].fov = fov;
    projectionViews[0].subImage.swapchain = xrLeftEyeSwapchain;
    projectionViews[0].subImage.imageArrayIndex = 0;
    projectionViews[0].subImage.imageRect.offset.x = 0;
    projectionViews[0].subImage.imageRect.offset.y = 0;
    projectionViews[0].subImage.imageRect.extent.width = 1344;
    projectionViews[0].subImage.imageRect.extent.height = 1600;



    projectionViews[1].type = XR_TYPE_COMPOSITION_LAYER_PROJECTION_VIEW;
    projectionViews[1].pose = defaultPose;
    projectionViews[1].fov = fov;
    projectionViews[1].subImage.swapchain = xrRightEyeSwapchain;
    projectionViews[1].subImage.imageArrayIndex = 0;
    projectionViews[1].subImage.imageRect.offset.x = 0;
    projectionViews[1].subImage.imageRect.offset.y = 0;
    projectionViews[1].subImage.imageRect.extent.width = 1344;
    projectionViews[1].subImage.imageRect.extent.height = 1600;



    projectionLayer.views = projectionViews;
    projectionLayer.viewCount = 2;
}




bool checkExtensionSupport(XrInstance instance) {
    uint32_t extensionCount = 0;
    XrResult result = xrEnumerateInstanceExtensionProperties(nullptr, 0, &extensionCount, nullptr);
    if (result != XR_SUCCESS) {
        throw std::runtime_error("Failed to query instance extension count.");
    }

    std::vector<XrExtensionProperties> extensionProperties(extensionCount, { XR_TYPE_EXTENSION_PROPERTIES });
    result = xrEnumerateInstanceExtensionProperties(nullptr, extensionCount, &extensionCount, extensionProperties.data());
    if (result != XR_SUCCESS) {
        throw std::runtime_error("Failed to enumerate instance extensions.");
    }

    bool extensionFound = false;
    for (const auto& extension : extensionProperties) {
        if (strcmp(extension.extensionName, XR_KHR_D3D11_ENABLE_EXTENSION_NAME) == 0) {
            extensionFound = true;
            break;
        }
    }

    if (!extensionFound) {
        throw std::runtime_error("Required extension XR_KHR_D3D11_enable is not supported.");
    }

    return true;
}

std::string debugInfo = "";

XrResult StringToPath(XrInstance instance, const char* pathStr, XrPath* path) {

    XrResult result = xrStringToPath(instance, pathStr, path);
    if (result != XR_SUCCESS) {
        debugInfo += pathStr;
    }

    return result;
}


#define XR_CURRENT_API_VERSION XR_MAKE_VERSION(1, 0, 28)

bool StartEngine::SetupXRInput() {

    if (!checkExtensionSupport(xrInstance)) {
        throw std::runtime_error("w");
    }

    std::string debugInfoPath = "D:/DEBUG_INFO_XR_YAY.txt";

    XrInstanceCreateInfo instanceCreateInfo{ XR_TYPE_INSTANCE_CREATE_INFO };
    strcpy_s(instanceCreateInfo.applicationInfo.applicationName, "Shipwreck 3D");
    instanceCreateInfo.applicationInfo.applicationVersion = 1;
    strcpy_s(instanceCreateInfo.applicationInfo.engineName, "Shipwreck Engine 3D");
    instanceCreateInfo.applicationInfo.engineVersion = 1;
    instanceCreateInfo.applicationInfo.apiVersion = XR_CURRENT_API_VERSION;

    uint32_t extensionCount = 1; // Count of extensions you want to enable
    const char* extensions[] = { XR_KHR_D3D11_ENABLE_EXTENSION_NAME, "XR_EXT_hand_tracking"};

    instanceCreateInfo.enabledExtensionCount = extensionCount;
    instanceCreateInfo.enabledExtensionNames = extensions;



    XrResult result = xrCreateInstance(&instanceCreateInfo, &xrInstance);
    if (XR_FAILED(result)) {
        throw std::runtime_error("xrCreateInstance(&instanceCreateInfo, &xrInstance)");
        return false;
    }

    XrSystemGetInfo systemInfo{ XR_TYPE_SYSTEM_GET_INFO };
    systemInfo.formFactor = XR_FORM_FACTOR_HEAD_MOUNTED_DISPLAY;

    XrSystemId systemId;
    result = xrGetSystem(xrInstance, &systemInfo, &systemId);
    if (XR_FAILED(result)) {
        throw std::runtime_error("w");
        return false;
    }
    
    if (isD3D) {
        PFN_xrGetD3D11GraphicsRequirementsKHR xrGetD3D11GraphicsRequirementsKHR = nullptr;
        xrGetInstanceProcAddr(xrInstance, "xrGetD3D11GraphicsRequirementsKHR", (PFN_xrVoidFunction*)&xrGetD3D11GraphicsRequirementsKHR);

        XrGraphicsRequirementsD3D11KHR graphicsRequirements{ XR_TYPE_GRAPHICS_REQUIREMENTS_D3D11_KHR };

        result = xrGetD3D11GraphicsRequirementsKHR(xrInstance, systemId, &graphicsRequirements);
        if (XR_FAILED(result)) {
            throw std::runtime_error("Failed to get D3D11 graphics requirements.");
            return false;
        }
        debugInfo += std::to_string(graphicsRequirements.minFeatureLevel);

        featureLevel = graphicsRequirements.minFeatureLevel;

        XrGraphicsBindingD3D11KHR d3d11Binding{ XR_TYPE_GRAPHICS_BINDING_D3D11_KHR };
        d3d11Binding.device = startEng.D3DGfx().device;
        
        XrSessionCreateInfo sessionCreateInfo{ XR_TYPE_SESSION_CREATE_INFO };
        sessionCreateInfo.next = &d3d11Binding;
        sessionCreateInfo.systemId = systemId;

        result = xrCreateSession(xrInstance, &sessionCreateInfo, &xrSession);
        if (XR_FAILED(result)) {
            throw std::runtime_error("result = xrCreateSession(xrInstance, &sessionCreateInfo, &xrSession);");
            return false;
        }
    }
    else {
        
        XrSessionCreateInfo sessionCreateInfo{ XR_TYPE_SESSION_CREATE_INFO };
        sessionCreateInfo.next = nullptr;
        sessionCreateInfo.systemId = systemId;

        result = xrCreateSession(xrInstance, &sessionCreateInfo, &xrSession);
        if (XR_FAILED(result)) {
            throw std::runtime_error("w");
            return false;
        }
    }


    XrActionSetCreateInfo actionSetCreateInfo{ XR_TYPE_ACTION_SET_CREATE_INFO };
    strcpy_s(actionSetCreateInfo.actionSetName, "input");
    strcpy_s(actionSetCreateInfo.localizedActionSetName, "Input Actions");
    result = xrCreateActionSet(xrInstance, &actionSetCreateInfo, &actionSet);
    if (XR_FAILED(result)) {
        throw std::runtime_error("Failed to create action set.");
    }

    /*PFN_xrCreateHandTrackerEXT xrCreateHandTrackerEXT = nullptr;
    xrGetInstanceProcAddr(xrInstance, "xrCreateHandTrackerEXT", (PFN_xrVoidFunction*)&xrCreateHandTrackerEXT);



    XrHandTrackerEXT handTracker;
    XrHandTrackerCreateInfoEXT handTrackerCreateInfo = { XR_TYPE_HAND_TRACKER_CREATE_INFO_EXT };
    handTrackerCreateInfo.next = nullptr;
    handTrackerCreateInfo.type = XR_TYPE_HAND_TRACKER;
    XrResult result = xrCreateHandTrackerEXT(xrSession, &handTrackerCreateInfo, &handTracker);
    if (result != XR_SUCCESS) {
        std::cerr << "Failed to create hand tracker." << std::endl;
        exit(-1);
    }*/




    // Create Actions
    auto createAction = [&](const char* name, const char* localized, XrActionType type, XrAction* action) {
        XrActionCreateInfo actionCreateInfo{ XR_TYPE_ACTION_CREATE_INFO };
        strcpy_s(actionCreateInfo.actionName, name);
        strcpy_s(actionCreateInfo.localizedActionName, localized);
        actionCreateInfo.actionType = type;
        return XR_FAILED(xrCreateAction(actionSet, &actionCreateInfo, action));
        };
    
    if (createAction("button_a", "Button A", XR_ACTION_TYPE_BOOLEAN_INPUT, &buttonAAction) ||
        createAction("button_b", "Button B", XR_ACTION_TYPE_BOOLEAN_INPUT, &buttonBAction) ||
        createAction("button_x", "Button X", XR_ACTION_TYPE_BOOLEAN_INPUT, &buttonXAction) ||
        createAction("button_y", "Button Y", XR_ACTION_TYPE_BOOLEAN_INPUT, &buttonYAction) ||
        createAction("left_trigger", "Left Trigger", XR_ACTION_TYPE_FLOAT_INPUT, &leftTriggerAction) ||
        createAction("right_trigger", "Right Trigger", XR_ACTION_TYPE_FLOAT_INPUT, &rightTriggerAction) ||
        createAction("left_thumbstick_x", "Left Thumbstick X", XR_ACTION_TYPE_FLOAT_INPUT, &leftXThumbstickAction) ||
        createAction("left_thumbstick_y", "Left Thumbstick Y", XR_ACTION_TYPE_FLOAT_INPUT, &leftYThumbstickAction) ||
        createAction("right_thumbstick_x", "Right Thumbstick X", XR_ACTION_TYPE_FLOAT_INPUT, &rightXThumbstickAction) ||
        createAction("right_thumbstick_y", "Right Thumbstick Y", XR_ACTION_TYPE_FLOAT_INPUT, &rightYThumbstickAction) ||
        createAction("left_grip", "Left Grip", XR_ACTION_TYPE_FLOAT_INPUT, &leftGripAction) ||
        createAction("right_grip", "Right Grip", XR_ACTION_TYPE_FLOAT_INPUT, &rightGripAction) ||
        createAction("menu_button", "Menu Button", XR_ACTION_TYPE_BOOLEAN_INPUT, &menuAction)) {
        throw std::runtime_error("Failed to create one or more actions.");
    }

    // Convert interaction profile string to XrPath
    XrPath interactionProfilePath;
    result = xrStringToPath(xrInstance, "/interaction_profiles/oculus/touch_controller", &interactionProfilePath);
    if (XR_FAILED(result)) {
        throw std::runtime_error("Failed to convert interaction profile string to path.");
    }

    // Convert action paths to XrPath
    auto StringToPath = [&](const char* pathStr, XrPath* path) {
        return xrStringToPath(xrInstance, pathStr, path);
        };

    XrPath buttonAPath, buttonBPath, buttonXPath, buttonYPath, leftTriggerPath, rightTriggerPath, leftXThumbstickPath, leftYThumbstickPath, rightXThumbstickPath, rightYThumbstickPath, leftGripPath, rightGripPath, menuPath;
    if (StringToPath("/user/hand/right/input/a/click", &buttonAPath) ||
        StringToPath("/user/hand/right/input/b/click", &buttonBPath) ||
        StringToPath("/user/hand/left/input/x/click", &buttonXPath) ||
        StringToPath("/user/hand/left/input/y/click", &buttonYPath) ||
        StringToPath("/user/hand/left/input/trigger/value", &leftTriggerPath) ||
        StringToPath("/user/hand/right/input/trigger/value", &rightTriggerPath) ||
        StringToPath("/user/hand/left/input/thumbstick/x", &leftXThumbstickPath) ||
        StringToPath("/user/hand/left/input/thumbstick/y", &leftYThumbstickPath) ||
        StringToPath("/user/hand/right/input/thumbstick/x", &rightXThumbstickPath) ||
        StringToPath("/user/hand/right/input/thumbstick/y", &rightYThumbstickPath) ||
        StringToPath("/user/hand/left/input/squeeze/value", &leftGripPath) ||
        StringToPath("/user/hand/right/input/squeeze/value", &rightGripPath) ||
        StringToPath("/user/hand/left/input/menu/click", &menuPath)) {
        throw std::runtime_error("Failed to convert action paths to XrPath.");
    }

    // Suggest Interaction Profile Bindings
    std::vector<XrActionSuggestedBinding> bindings = {
        {buttonAAction, buttonAPath},
        {buttonBAction, buttonBPath},
        {buttonXAction, buttonXPath},
        {buttonYAction, buttonYPath},
        {leftTriggerAction, leftTriggerPath},
        {rightTriggerAction, rightTriggerPath},
        {leftXThumbstickAction, leftXThumbstickPath},
        {leftYThumbstickAction, leftYThumbstickPath},
        {rightXThumbstickAction, rightXThumbstickPath},
        {rightYThumbstickAction, rightYThumbstickPath},
        {leftGripAction, leftGripPath},
        {rightGripAction, rightGripPath},
        {menuAction, menuPath}
    };

    XrInteractionProfileSuggestedBinding suggestedBinding{ XR_TYPE_INTERACTION_PROFILE_SUGGESTED_BINDING };
    suggestedBinding.interactionProfile = interactionProfilePath;
    suggestedBinding.suggestedBindings = bindings.data();
    suggestedBinding.countSuggestedBindings = static_cast<uint32_t>(bindings.size());

    result = xrSuggestInteractionProfileBindings(xrInstance, &suggestedBinding);
    if (XR_FAILED(result)) {
        std::string errorMessage = "Failed to suggest interaction profile bindings: ";

        switch (result) {
        case XR_ERROR_VALIDATION_FAILURE:
            errorMessage += "Validation failure.";
            break;
        case XR_ERROR_RUNTIME_FAILURE:
            errorMessage += "Runtime failure.";
            break;
        case XR_ERROR_INSTANCE_LOST:
            errorMessage += "Instance lost.";
            break;
        case XR_ERROR_SESSION_LOST:
            errorMessage += "Session lost.";
            break;
        case XR_ERROR_HANDLE_INVALID:
            errorMessage += "Handle invalid.";
            break;
        case XR_ERROR_ACTIONSETS_ALREADY_ATTACHED:
            errorMessage += "Action sets already attached.";
            break;
        default:
            errorMessage += "Unknown error code: " + std::to_string(result);
            break;
        }

        WriteDataToFile(debugInfoPath, errorMessage);
        throw std::runtime_error(errorMessage);
    }

    // Attach Action Sets to Session
    XrSessionActionSetsAttachInfo attachInfo{ XR_TYPE_SESSION_ACTION_SETS_ATTACH_INFO };
    attachInfo.countActionSets = 1;
    attachInfo.actionSets = &actionSet;
    result = xrAttachSessionActionSets(xrSession, &attachInfo);
    if (XR_FAILED(result)) {
        throw std::runtime_error("Failed to attach action sets to session.");
    }






    //Swap Chain
    XrSwapchainCreateInfo swapchainCreateInfo = {};
    swapchainCreateInfo.type = XR_TYPE_SWAPCHAIN_CREATE_INFO;
    swapchainCreateInfo.format = DXGI_FORMAT_B8G8R8A8_UNORM; // or your preferred format
    swapchainCreateInfo.sampleCount = 1;
    swapchainCreateInfo.width = 1344;
    swapchainCreateInfo.height = 1600;
    swapchainCreateInfo.faceCount = 1;
    swapchainCreateInfo.arraySize = 1;
    swapchainCreateInfo.mipCount = 1;
    swapchainCreateInfo.usageFlags = XR_SWAPCHAIN_USAGE_COLOR_ATTACHMENT_BIT;

    // Create swapchains for both eyes
    result = xrCreateSwapchain(xrSession, &swapchainCreateInfo, &xrLeftEyeSwapchain);
    if (XR_FAILED(result)) {
        throw std::runtime_error("Failed to attach action sets to session.");
    }

    result = xrCreateSwapchain(xrSession, &swapchainCreateInfo, &xrRightEyeSwapchain);
    if (XR_FAILED(result)) {
        throw std::runtime_error("Failed to attach action sets to session.");
    }

    SetupXRLayers();

    return true;
}

void StartEngine::ProcessXRInput() {

    XrActiveActionSet activeActionSet{ actionSet, XR_NULL_PATH };
    XrActionsSyncInfo syncInfo{ XR_TYPE_ACTIONS_SYNC_INFO };
    syncInfo.countActiveActionSets = 1;
    syncInfo.activeActionSets = &activeActionSet;

    //Update Actions
    XrResult result = xrSyncActions(xrSession, &syncInfo);
    if (XR_FAILED(result)) {
        return;
    }

    XrActionStateGetInfo getInfo{ XR_TYPE_ACTION_STATE_GET_INFO };

    //A button
    XrActionStateBoolean buttonAState{ XR_TYPE_ACTION_STATE_BOOLEAN };
    getInfo.action = buttonAAction;
    xrGetActionStateBoolean(xrSession, &getInfo, &buttonAState);
    if (buttonAState.changedSinceLastSync) {
        if (buttonAState.currentState) {
            xrInput.OnButtonAPressed();
        }
        else {
            xrInput.OnButtonAReleased();
        }
    }

    //B button
    XrActionStateBoolean buttonBState{ XR_TYPE_ACTION_STATE_BOOLEAN };
    getInfo.action = buttonBAction;
    xrGetActionStateBoolean(xrSession, &getInfo, &buttonBState);
    if (buttonBState.changedSinceLastSync) {
        if (buttonBState.currentState) {
            xrInput.OnButtonBPressed();
        }
        else {
            xrInput.OnButtonBReleased();
        }
    }

    //X button
    XrActionStateBoolean buttonXState{ XR_TYPE_ACTION_STATE_BOOLEAN };
    getInfo.action = buttonXAction;
    xrGetActionStateBoolean(xrSession, &getInfo, &buttonXState);
    if (buttonXState.changedSinceLastSync) {
        if (buttonBState.currentState) {
            xrInput.OnButtonXPressed();
        }
        else {
            xrInput.OnButtonXReleased();
        }
    }

    //Y button
    XrActionStateBoolean buttonYState{ XR_TYPE_ACTION_STATE_BOOLEAN };
    getInfo.action = buttonYAction;
    xrGetActionStateBoolean(xrSession, &getInfo, &buttonYState);
    if (buttonYState.changedSinceLastSync) {
        if (buttonYState.currentState) {
            xrInput.OnButtonYPressed();
        }
        else {
            xrInput.OnButtonYReleased();
        }
    }


    //Menu button
    XrActionStateBoolean menuState{ XR_TYPE_ACTION_STATE_BOOLEAN };
    getInfo.action = menuAction;
    xrGetActionStateBoolean(xrSession, &getInfo, &menuState);
    if (menuState.changedSinceLastSync) {
        if (menuState.currentState) {
            xrInput.OnMenuPressed();
        }
        else {
            xrInput.OnMenuReleased();
        }
    }


    //Left Trigger float
    XrActionStateFloat leftTriggerState{ XR_TYPE_ACTION_STATE_FLOAT };
    getInfo.action = leftTriggerAction;
    xrGetActionStateFloat(xrSession, &getInfo, &leftTriggerState);
    if (leftTriggerState.changedSinceLastSync) {
        if (leftTriggerState.currentState > 0.0f) {
            xrInput.OnLeftTriggerPressed(leftTriggerState.currentState);
        }
        else {
            xrInput.OnLeftTriggerReleased();
        }
    }

    //Right Trigger float
    XrActionStateFloat rightTriggerState{ XR_TYPE_ACTION_STATE_FLOAT };
    getInfo.action = rightTriggerAction;
    xrGetActionStateFloat(xrSession, &getInfo, &rightTriggerState);
    if (rightTriggerState.changedSinceLastSync) {
        if (rightTriggerState.currentState > 0.0f) {
            xrInput.OnRightTriggerPressed(rightTriggerState.currentState);
        }
        else {
            xrInput.OnRightTriggerReleased();
        }
    }


    //Left Grip float
    XrActionStateFloat leftGripState{ XR_TYPE_ACTION_STATE_FLOAT };
    getInfo.action = leftGripAction;
    xrGetActionStateFloat(xrSession, &getInfo, &leftGripState);
    if (leftGripState.changedSinceLastSync) {
        if (leftGripState.currentState > 0.0f) {
            xrInput.OnLeftGripPressed(leftGripState.currentState);
        }
        else {
            xrInput.OnLeftGripReleased();
        }
    }

    //Right Grip float
    XrActionStateFloat rightGripState{ XR_TYPE_ACTION_STATE_FLOAT };
    getInfo.action = rightGripAction;
    xrGetActionStateFloat(xrSession, &getInfo, &rightGripState);
    if (rightGripState.changedSinceLastSync) {
        if (rightGripState.currentState > 0.0f) {
            xrInput.OnRightGripPressed(rightGripState.currentState);
        }
        else {
            xrInput.OnRightGripReleased();
        }
    }


    //Left Thumbstick axes
    XrActionStateFloat leftXThumbstickState{ XR_TYPE_ACTION_STATE_FLOAT };
    getInfo.action = leftXThumbstickAction;
    xrGetActionStateFloat(xrSession, &getInfo, &leftXThumbstickState);

    XrActionStateFloat leftYThumbstickState{ XR_TYPE_ACTION_STATE_FLOAT };
    getInfo.action = leftYThumbstickAction;
    xrGetActionStateFloat(xrSession, &getInfo, &leftYThumbstickState);

    if (leftXThumbstickState.changedSinceLastSync || leftYThumbstickState.changedSinceLastSync) {
        xrInput.OnLeftThumbstickMove(leftXThumbstickState.currentState, leftYThumbstickState.currentState);
    }

    //Right Thumbstick axes
    XrActionStateFloat rightXThumbstickState{ XR_TYPE_ACTION_STATE_FLOAT };
    getInfo.action = rightXThumbstickAction;
    xrGetActionStateFloat(xrSession, &getInfo, &rightXThumbstickState);

    XrActionStateFloat rightYThumbstickState{ XR_TYPE_ACTION_STATE_FLOAT };
    getInfo.action = rightYThumbstickAction;
    xrGetActionStateFloat(xrSession, &getInfo, &rightYThumbstickState);

    if (rightXThumbstickState.changedSinceLastSync || rightYThumbstickState.changedSinceLastSync) {
        xrInput.OnRightThumbstickMove(rightXThumbstickState.currentState, rightYThumbstickState.currentState);
    }
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

        if (containers[i]->rigidbody.getDynamic()) {
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
                loadedSegment = "";
                curContainer = Container();
                //loadedContainers.emplace_back();
            }
            curlyBracket++;
            continue;
            //create container and add the pointer to containers
        }

        if (curChar == '}' && !inQuote) {
            if (curlyBracket == 1) {
                loadedSegment = "";
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

            if (loadedSegment == "Sprite" && !inClassSetup && !inQuote) {
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
                        //curContainer.rigidbody.isDynamic = true;
                    }
                    else if (loadedSegment == "f") {
                        //curContainer.rigidbody.isDynamic = false;
                    }
                }
                else if (classParameters == 1) {
                    if (loadedSegment == "t") {
                        //curContainer.rigidbody.NewRB2D();
                    }
                    else if (loadedSegment == "f") {
                        //curContainer.rigidbody.NewRB();
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

                    std::string modelPath = RemoveQuotes(loadedSegment);

                    //modelImporter.ImportModel((char*)modelPath.c_str(), &texLoader);

                    curContainer.models.modelList.emplace_back();
                    curContainer.models.modelList.back().ImportModel((char*)modelPath.c_str(), &texLoader);
                    curContainer.models.modelList.back().modelPath = modelPath;
                }
                //loadedSegment
            }



            //if (loadedClass == "Sprite" && !inQuote) {

            //    if (curChar == '\"' || curChar == '\'') {

            //        std::string spritePath = RemoveQuotes(loadedSegment);

            //        curContainer.sprites.sprites.push_back();
            //        curContainer.sprites.sprites.back().spritePath = spritePath;
            //    }
            //    //loadedSegment
            //}


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


void UpdateWindowSize() {

    RECT rect;

    GetWindowRect(hWnd, &rect);


    windowWidth = (rect.right - rect.left);
    windowHeight = (rect.bottom - rect.top);
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

        input.mouse = &startEng.GetMouse();
        input.keyboard = &startEng.GetKeyboard();


        //Mouse Section:
        glfwSetWindowUserPointer(window, &input);

        glfwSetMouseButtonCallback(window, startEng.GLGfx().mouseClickCallback);
        glfwSetCursorPosCallback(window, startEng.GLGfx().mouseMoveCallback);
        glfwSetScrollCallback(window, startEng.GLGfx().scrollCallback);
        
        
        //Keyboard Section:
        glfwSetWindowUserPointer(window, &input);
        
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
            exitedProgram = true;
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


        switch (wParam) {
        case VK_F11:
            ToggleFullscreen();
        }


        /*static WindowsMessageMap mm;
        OutputDebugString((LPCWSTR)mm(msg, lParam, wParam).c_str());
        
        Use: TEXT("Example Text"),   to convert from const char* (string) to LPCWCHAR or a string of text
        
        */

        startEng.keyboard.OnKeyDown(wParam);
    }
    case WM_SIZE:
    {
        if (isD3D) {

            if (startEng.postD3DGraphics) {
                if (startEng.D3DGfx().target != nullptr) {

                    /*if (startEng.xrLeftEyeSwapchain) {
                        xrDestroySwapchain(startEng.xrLeftEyeSwapchain);
                        startEng.xrLeftEyeSwapchain = nullptr;
                    }
                    if (startEng.xrRightEyeSwapchain) {
                        xrDestroySwapchain(startEng.xrRightEyeSwapchain);
                        startEng.xrRightEyeSwapchain = nullptr;
                    }

                    
                    XrSwapchainCreateInfo swapchainCreateInfo = {};
                    swapchainCreateInfo.type = XR_TYPE_SWAPCHAIN_CREATE_INFO;
                    swapchainCreateInfo.format = DXGI_FORMAT_B8G8R8A8_UNORM;
                    swapchainCreateInfo.width = 1344;
                    swapchainCreateInfo.height = 1600;
                    swapchainCreateInfo.sampleCount = 1;
                    swapchainCreateInfo.faceCount = 1;
                    swapchainCreateInfo.arraySize = 1;
                    swapchainCreateInfo.mipCount = 1;
                    swapchainCreateInfo.usageFlags = XR_SWAPCHAIN_USAGE_COLOR_ATTACHMENT_BIT;

                    xrCreateSwapchain(startEng.xrSession, &swapchainCreateInfo, &startEng.xrLeftEyeSwapchain);
                    xrCreateSwapchain(startEng.xrSession, &swapchainCreateInfo, &startEng.xrRightEyeSwapchain);*/

                    windowWidth = LOWORD(lParam);
                    windowHeight = HIWORD(lParam);

                    switch (wParam) {
                    case SIZE_RESTORED:
                    case SIZE_MAXIMIZED:
                        startEng.D3DGfx().RefreshWindow(hWnd);
                        break;
                    case SIZE_MINIMIZED:

                        break;
                    default:
                        break;
                    }
                    //startEng.D3DGfx().RefreshWindow(hWnd);
                }
            }
        }
    }
    break;

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