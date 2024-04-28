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





#include <dxtex/DirectXTex.h>




#include "StartEngine.h"
#include "Application.h"



#include "testing.h"



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


HWND hWnd;


StartEngine startEng;
FontCreation::SetupFace *setupFace;
Texture newTexture;

Textbox newText;

Camera camera;


Physics physics;


ModelImporter modelImporter;

GLFWwindow* window;

bool exited = false;
bool isD3D = true;

int targetFPS = 240;
std::chrono::milliseconds targetFrameLength(1000 / targetFPS);

void DisplayVarAsTitle(float newVar);
void DisplayVarAsTitle(float newVar) {

    std::ostringstream oss;
    oss << "variable: " << newVar;


    std::string widthStr = oss.str();

    //MessageBoxA(hWnd, widthStr.c_str(), "Hey", 0);
    SetWindowTextA(hWnd, widthStr.c_str());
}


//WM_KEYDOWN
int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {

    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);


    // Initialize global strings
    LoadString(hInstance, APP_NAME, szTitle, MAX_LOADSTRING);
    LoadString(hInstance, PROJ_FILENAME, szWindowClass, MAX_LOADSTRING);

    startEng.isD3D = isD3D;

    startEng.timeManager.StartTime();

    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance(hInstance, nCmdShow, Vec2(900,900)))
    {
        return FALSE;
    }

    /*StartEngine gfx;*/
    if (isD3D) {
        startEng.postD3DGraphics = std::make_unique<D3DGraphics>(hWnd);
    }
    else {
        startEng.postGLGraphics = std::make_unique<GLGraphics>();
    }


    setupFace = &startEng.Font().LoadFont("C:/Windows/Fonts/Arial.ttf");
    //setupFace = &startEng.Font().LoadFont("D:/Shipwreck Engine 3D/Shipwreck Engine 3D/Myfont-Regular.ttf");


    newText.textToLoad = /*"Shipwreck 3D" */ /*"The quick brown fox jumps over the lazy dog"*/ "abcdefghijklmnopqrstuvwxyz" /*"igiaiii"*/; //szTitle for title text
    newText.fontManager = setupFace;
    newText.textRect = Vec2(455.0f, 0.0f);
    newText.textPos = Vec2(-220.0f, 0.0f);
    newText.fontSize = 12.0f;
    newText.hWnd = hWnd;

    newText.LoadText();





    if (!physics.SetupPhysics()) {
        WPARAM wp = 1111;
        LPARAM lp = 1000;
        SendMessageA(hWnd, Msgbox, wp, lp);
    }
    if (!physics.TestPhysics()) {
        WPARAM wp = 1001;
        LPARAM lp = 1000;
        SendMessageA(hWnd, Msgbox, wp, lp);
    }




    //newTexture.LoadTexture("C:/Users/smsal/OneDrive/Documents/Game Art/Cave block.png");
    //newTexture.LoadTexture("C:/Users/smsal/OneDrive/Documents/Game Art/Untitled_Artwork.png");
    //newTexture.LoadTexture("C:/Users/smsal/OneDrive/Documents/Game Art/Tree.png");
    //newTexture.LoadTexture("C:/Users/smsal/OneDrive/Documents/Game Art/Other/Platforms.png");
    //newTexture.LoadTexture("C:/Users/smsal/OneDrive/Documents/Game Art/Other/Player.png");
    newTexture.LoadTexture("C:/Users/smsal/OneDrive/Documents/Game Art/Mask Game/Character/Character-1.png");

    //modelImporter.ImportModel("C:/Users/smsal/OneDrive/Documents/Blender Modules/cube.fbx");
    //modelImporter.ImportModel("C:/Users/smsal/OneDrive/Documents/Blender Modules/sphere.obj");
    modelImporter.ImportModel("C:/Users/smsal/OneDrive/Documents/Blender Modules/sphereV2.fbx");
    //modelImporter.ImportModel("C:/Users/smsal/OneDrive/Documents/Blender Modules/bow.fbx");
    //modelImporter.ImportModel("C:/Users/smsal/OneDrive/Documents/Blender Modules/handV2.fbx");
    //modelImporter.ImportModel("C:/Users/smsal/OneDrive/Documents/Blender Modules/handV2.obj");


    startEng.RenderFrame();

    MSG msg;

    // Main message loop:
    while (!exited && !glfwWindowShouldClose(window))
    {
        auto frameStart = std::chrono::high_resolution_clock::now();
        /*
        if (GetMessage(&msg, nullptr, 0, 0) <= 0) {

        }*/
        if (PeekMessage(&msg, hWnd, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else {
            startEng.RenderFrame();
            

            if (!startEng.mouse.IsEmpty()) {
               /* const auto mouseEvent = startEng.mouse.ReadMouse();

                if (mouseEvent.GetMouseState() == Mouse::MouseEvent::State::Button0Down) {
                    SendMessageA(hWnd, Msgbox, 0, 0);
                }*/

                if (startEng.mouse.Button0) {
                    /*WPARAM wp = 0;
                    LPARAM lp = 0;
                    SendMessage(hWnd, Msgbox, wp, lp);*/
                    WPARAM wp = 0;
                    LPARAM lp = 0;
                    SendMessage(hWnd, Msgbox, wp, lp);
                }

                if (startEng.mouse.Button1) {
                    WPARAM wp = 1;
                    LPARAM lp = 1;
                    SendMessage(hWnd, Msgbox, wp, lp);
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
            if (startEng.keyboard.GetKeyState('H')) {
                MessageBox(hWnd, L"H", L"Hey", 0);
            }

            startEng.mouse.UpdateMouse(); //updates scroll wheel delta,   MAYBE: add a mouse movement delta, to add support for mouse movement per frame

            auto frameEnd = std::chrono::high_resolution_clock::now();
            auto frameDuration = frameEnd - frameStart;

            if (frameDuration < targetFrameLength) {
                std::this_thread::sleep_for(targetFrameLength - frameDuration);
            }
        }



        //use '[key letter]' for letters
        //use VK_[key name] for accelerator keys like [CTRL], [SPACE], and [ALT]
    }
    if (!isD3D) {
        glfwDestroyWindow(window);
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
    
    if (isD3D) {
        D3DGfx().ClearBuffer(0.4f, 0.4f, 0.4f);
        //Gfx().DrawRect(Vec2(0.0f, 0.0f), Vec2(1.0f, 1.0f), 19, 0.5f /*0.2f*/, Color(30, 30, 30, 255), hWnd);
        float rotTimer = startEng.timeManager.GetTime() / 1000;




        RECT rect;
        GetWindowRect(hWnd, &rect);
        int width = (rect.right - rect.left);
        int height = (rect.bottom - rect.top);



        physics.Simulate();


        //PxActor** actors;
        //physics.scene->getActors(PxActorTypeFlag::eRIGID_DYNAMIC,actors,1); //actors[0]->is<PxRigidDynamic>()->getGlobalPose().p.z


        PxVec3 pos = physics.rigidbody->getGlobalPose().p;
        PxQuat rotQuat = physics.rigidbody->getGlobalPose().q;
        PxVec3 rot = rotQuat.getBasisVector1();


        PxVec3 pos2 = physics.rigidbody2->getGlobalPose().p;
        PxQuat rot2Quat = physics.rigidbody2->getGlobalPose().q;
        PxVec3 rot2 = rot2Quat.getBasisVector1();

        //rot2Quat.toRadiansAndUnitAxis(angle2, rot2);
        //radians to degrees conversion (57.2958 degrees is 1 radian)
        /*rot2 *= angle2;
        rot2 /= 57.2958f;*/




        //Gfx().DrawTestCube(Color(10, 170, 170, 255), Vec3(pos.x, pos.y, pos.z), Rotation(rot.x, rot.y, rot.z), hWnd);
        D3DGfx().DrawTestCube(Color(40, 170, 70, 255), Vec3(pos2.x, pos2.y, pos2.z), Rotation(rot2.x, rot2.y, rot2.z), camera, hWnd);

        D3DGfx().RenderModel(modelImporter.meshes[0], Vec3(pos.x, pos.y, pos.z), Rotation(rot.x, rot.y, rot.z), camera, hWnd); //Rotation(-mouse.mousePosition.y / 100, -mouse.mousePosition.x / 100,0.0f)

        //Gfx().RenderModel(modelImporter.meshes[0], Vec3(pos2.x, pos2.y, pos2.z), Rotation(rot2.x, rot2.y, rot2.z), hWnd); //Rotation(-mouse.mousePosition.y / 100, -mouse.mousePosition.x / 100,0.0f)



        //D3DGfx().DrawSprite(Vec2(0.0f, 0.0f), Vec2(1.0f, 1.0f), &newTexture, hWnd);


        D3DGfx().ShowText(newText/*Textbox("igiaiii", setupFace, Vec2(55.0f, 0.0f), Vec2(100.0f, 0.0f), 12, hWnd)*/);

        //Gfx().DrawRect(Vec2(0.0f, 0.0f), Vec2(1.0f, 1.0f), 0, 0 /*0.2f*/, Color(30, 30, 30, 255), hWnd);

        //Gfx().DrawTestCube(Color(255, 0, 0, 255), Rotation(rotTimer,rotTimer,rotTimer), hWnd);
        //Gfx().DrawTestCube(Color(255, 0, 0, 255), Rotation(rotTimer + 50,rotTimer + 50,rotTimer + 50), hWnd);

        D3DGfx().EndFrame();
    }
    else {
        physics.Simulate();


        //PxActor** actors;
        //physics.scene->getActors(PxActorTypeFlag::eRIGID_DYNAMIC,actors,1); //actors[0]->is<PxRigidDynamic>()->getGlobalPose().p.z


        PxVec3 pos = physics.rigidbody->getGlobalPose().p;
        PxQuat rotQuat = physics.rigidbody->getGlobalPose().q;
        PxVec3 rot = rotQuat.getBasisVector1();


        PxVec3 pos2 = physics.rigidbody2->getGlobalPose().p;
        PxQuat rot2Quat = physics.rigidbody2->getGlobalPose().q;
        PxVec3 rot2 = rot2Quat.getBasisVector1();

        //rot2Quat.toRadiansAndUnitAxis(angle2, rot2);
        //radians to degrees conversion (57.2958 degrees is 1 radian)
        /*rot2 *= angle2;
        rot2 /= 57.2958f;*/

        GLGfx().ClearBuffer(0.3f, 0.3f, 0.3f);
        //GLGfx().DrawTestTri();
        //GLGfx().DrawTestCube(Color(40, 170, 70, 255), Vec3(pos.x, pos.y, pos.z), Rotation(rot.x, rot.y, rot.z), camera, window);
        //GLGfx().DrawTestCube(Color(40, 170, 70, 255), Vec3(pos2.x, pos2.y, pos2.z), Rotation(rot2.x, rot2.y, rot2.z), camera, window);
        GLGfx().RenderModel(modelImporter.meshes[0], Vec3(pos.x, pos.y, pos.z), Rotation(rot.x, rot.y, rot.z), camera, window);

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

D3DGraphics& StartEngine::D3DGfx() {
     return *postD3DGraphics;
}

GLGraphics& StartEngine::GLGfx() {
    return *postGLGraphics;
}


FontCreation& StartEngine::Font() {
    return *fontManagement;
}

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
        hWnd = CreateWindow(szWindowClass, szTitle, WS_POPUP, //WS_POPUP,
            (screenRect.right / 2) - (windSize.x / 2), (screenRect.bottom / 2) - (windSize.y / 2), windSize.x, windSize.y, nullptr, nullptr, hInstance, nullptr);


        if (!hWnd)
        {
            return FALSE;
        }

        ShowWindow(hWnd, nCmdShow);
        UpdateWindow(hWnd);
    }
    else {
        glfwInit();
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

        glfwSetFramebufferSizeCallback(window, GLGraphics::framebuffer_size_callback);
    }



    return TRUE;
}

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
        startEng.keyboard.OnKeyUp(wParam);
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

            startEng.mouse.OnMouseMove(pts.x,pts.y);

            //mouse in window
            if (!startEng.mouse.inWindow) {

                SetCapture(hWnd);

                startEng.mouse.OnMouseEnter();
            }
        }
        else {
            if ( startEng.mouse.Button0 & (MK_LBUTTON | MK_RBUTTON)) {

                startEng.mouse.OnMouseMove(pts.x, pts.y);
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