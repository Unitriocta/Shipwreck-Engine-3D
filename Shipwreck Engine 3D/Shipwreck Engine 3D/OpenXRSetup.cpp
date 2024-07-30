#include "OpenXRSetup.h"

#include "StartEngine.h"
using namespace EngineInstance;



OpenXRSetup::OpenXRSetup() : //1080 x 1200, 1344 x 1600
    canRenderToXR(false), swapchainWidth(1344), swapchainHeight(1600), xrInstance(XR_NULL_HANDLE), xrSession(XR_NULL_HANDLE), systemId(XR_NULL_SYSTEM_ID),
    referenceSpace(XR_NULL_HANDLE), xrLeftEyeSwapchain(XR_NULL_HANDLE), xrRightEyeSwapchain(XR_NULL_HANDLE), actionSet(XR_NULL_HANDLE), colorFormat(DXGI_FORMAT_R8G8B8A8_UNORM),

    buttonAAction(XR_NULL_HANDLE), buttonBAction(XR_NULL_HANDLE), buttonXAction(XR_NULL_HANDLE), buttonYAction(XR_NULL_HANDLE),
    leftTriggerAction(XR_NULL_HANDLE), rightTriggerAction(XR_NULL_HANDLE), leftXThumbstickAction(XR_NULL_HANDLE),
    leftYThumbstickAction(XR_NULL_HANDLE), rightXThumbstickAction(XR_NULL_HANDLE), rightYThumbstickAction(XR_NULL_HANDLE),
    leftGripAction(XR_NULL_HANDLE), rightGripAction(XR_NULL_HANDLE), menuAction(XR_NULL_HANDLE)
{}





bool OpenXRSetup::SetupXR() {

    if (!SetupInstance()) {
        return false;
    }

    if (!LoadSystem()) {
        return false;
    }

    if (!SetupSession()) {
        return false;
    }

    if (!SetupInput()) {
        return false;
    }

    if (!SetupGraphics()) {
        return false;
    }

    if (!SetupXRLayers()) {
        return false;
    }

    /*if (!StartSession()) {
        return false;
    }*/



    return true;
}


void OpenXRSetup::PollEvents() {

    XrResult result;


    XrEventDataBuffer eventData = { XR_TYPE_EVENT_DATA_BUFFER };

    while (xrPollEvent(xrInstance, &eventData) == XR_SUCCESS) {
        if (eventData.type == XR_TYPE_EVENT_DATA_SESSION_STATE_CHANGED) {
            XrEventDataSessionStateChanged* sessionStateChanged =
                reinterpret_cast<XrEventDataSessionStateChanged*>(&eventData);


            // Output or handle the session state
            XrSessionState state = sessionStateChanged->state;
            switch (state) {
            case XR_SESSION_STATE_IDLE:
                if (xrSessionStarted) {

                    throw std::runtime_error("w");
                }
                break;
            case XR_SESSION_STATE_READY:

                if (!xrSessionStarted) {
                    StartSession();
                    xrSessionStarted = true;
                }
                canRenderToXR = true;

                break;
            case XR_SESSION_STATE_SYNCHRONIZED:
                break;
            case XR_SESSION_STATE_VISIBLE:
                canRenderToXR = true;
                break;
            case XR_SESSION_STATE_FOCUSED:
                canRenderToXR = true;
                break;
            case XR_SESSION_STATE_STOPPING:
                canRenderToXR = false;
                break;
            case XR_SESSION_STATE_EXITING:

                if (xrSessionStarted) {

                    result = xrEndSession(xrSession);
                    if (XR_FAILED(result)) {
                        throw std::runtime_error("w");
                    }

                    xrSessionStarted = false;
                    canRenderToXR = false;
                }

                break;
            case XR_SESSION_STATE_LOSS_PENDING:
                throw std::runtime_error("w");
                break;
            case XR_SESSION_STATE_UNKNOWN:

                break;
            default:
                break;
            }
        }
    }
}



bool OpenXRSetup::SetupInstance() {

    /*if (!checkExtensionSupport(xrInstance, XR_KHR_D3D11_ENABLE_EXTENSION_NAME)) {
        throw std::runtime_error("w");
    }*/
    /*if (!checkExtensionSupport(xrInstance, XR_EXT_HAND_TRACKING_EXTENSION_NAME)) {
        throw std::runtime_error("w");
    }*/

    XrInstanceCreateInfo instanceCreateInfo{ XR_TYPE_INSTANCE_CREATE_INFO };
    strcpy_s(instanceCreateInfo.applicationInfo.applicationName, "Shipwreck 3D");
    instanceCreateInfo.applicationInfo.applicationVersion = 1;
    strcpy_s(instanceCreateInfo.applicationInfo.engineName, "Shipwreck Engine 3D");
    instanceCreateInfo.applicationInfo.engineVersion = 1;
    instanceCreateInfo.applicationInfo.apiVersion = XR_CURRENT_API_VERSION;

    uint32_t extensionCount = 1; // Count of extensions you want to enable
    const char* extensions[] = { XR_KHR_D3D11_ENABLE_EXTENSION_NAME/*, XR_EXT_HAND_TRACKING_EXTENSION_NAME*/ };

    instanceCreateInfo.enabledExtensionCount = extensionCount;
    instanceCreateInfo.enabledExtensionNames = extensions;



    XrResult result = xrCreateInstance(&instanceCreateInfo, &xrInstance);
    if (XR_FAILED(result)) {
        throw std::runtime_error("xrCreateInstance(&instanceCreateInfo, &xrInstance)");
        return false;
    }

    return true;
}

bool OpenXRSetup::LoadSystem() {

    XrResult result;


    XrSystemGetInfo systemInfo{ XR_TYPE_SYSTEM_GET_INFO };
    systemInfo.formFactor = XR_FORM_FACTOR_HEAD_MOUNTED_DISPLAY;


    result = xrGetSystem(xrInstance, &systemInfo, &systemId);
    if (XR_FAILED(result)) {
        throw std::runtime_error("w");
        return false;
    }

    return true;
}

bool OpenXRSetup::SetupSession() {

    XrResult result;


    if (isD3D) {
        result = xrGetInstanceProcAddr(xrInstance, "xrGetD3D11GraphicsRequirementsKHR", (PFN_xrVoidFunction*)&xrGetD3D11GraphicsRequirementsKHR);
        if (XR_FAILED(result)) {
            throw std::runtime_error("Failed to get D3D11 graphics requirements.");
            return false;
        }

        xrGraphicsRequirements = { XR_TYPE_GRAPHICS_REQUIREMENTS_D3D11_KHR };

        result = xrGetD3D11GraphicsRequirementsKHR(xrInstance, systemId, &xrGraphicsRequirements);
        if (XR_FAILED(result)) {
            throw std::runtime_error("Failed to get D3D11 graphics requirements.");
            return false;
        }

        featureLevel = xrGraphicsRequirements.minFeatureLevel;

        xrD3d11Binding = { XR_TYPE_GRAPHICS_BINDING_D3D11_KHR };
        xrD3d11Binding.device = startEng.D3DGfx().device;
        xrD3d11Binding.next = nullptr;

        XrSessionCreateInfo sessionCreateInfo{ XR_TYPE_SESSION_CREATE_INFO };
        sessionCreateInfo.next = &xrD3d11Binding;
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

    return true;
}

bool OpenXRSetup::StartSession() {

    XrResult result;

    XrSessionBeginInfo beginInfo = {};
    beginInfo.type = XR_TYPE_SESSION_BEGIN_INFO;
    beginInfo.next = nullptr;
    beginInfo.primaryViewConfigurationType = XR_VIEW_CONFIGURATION_TYPE_PRIMARY_STEREO;

    result = xrBeginSession(xrSession, &beginInfo);
    if (XR_FAILED(result)) {

        throw std::runtime_error("w");
        return false;
    }

    return true;
}




//Input:
bool OpenXRSetup::SetupInput() {

    XrResult result;


    XrActionSetCreateInfo actionSetCreateInfo{ XR_TYPE_ACTION_SET_CREATE_INFO };
    strcpy_s(actionSetCreateInfo.actionSetName, "input");
    strcpy_s(actionSetCreateInfo.localizedActionSetName, "Input Actions");
    result = xrCreateActionSet(xrInstance, &actionSetCreateInfo, &actionSet);
    if (XR_FAILED(result)) {
        throw std::runtime_error("Failed to create action set.");
        return false;
    }



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
        return false;
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
        return false;
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

        //WriteDataToFile(debugInfoPath, errorMessage);
        throw std::runtime_error(errorMessage);
        return false;
    }

    // Attach Action Sets to Session
    XrSessionActionSetsAttachInfo attachInfo{ XR_TYPE_SESSION_ACTION_SETS_ATTACH_INFO };
    attachInfo.countActionSets = 1;
    attachInfo.actionSets = &actionSet;
    result = xrAttachSessionActionSets(xrSession, &attachInfo);
    if (XR_FAILED(result)) {
        throw std::runtime_error("Failed to attach action sets to session.");
        return false;
    }

    return true;
}


void OpenXRSetup::ProcessFrameInput() {

    XrPath touchControllerPath;
    xrStringToPath(xrInstance, "/interaction_profiles/oculus/touch_controller", &touchControllerPath);

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
            startEng.xrInput.OnButtonAPressed();
        }
        else {
            startEng.xrInput.OnButtonAReleased();
        }
    }

    //B button
    XrActionStateBoolean buttonBState{ XR_TYPE_ACTION_STATE_BOOLEAN };
    getInfo.action = buttonBAction;
    xrGetActionStateBoolean(xrSession, &getInfo, &buttonBState);
    if (buttonBState.changedSinceLastSync) {
        if (buttonBState.currentState) {
            startEng.xrInput.OnButtonBPressed();
        }
        else {
            startEng.xrInput.OnButtonBReleased();
        }
    }

    //X button
    XrActionStateBoolean buttonXState{ XR_TYPE_ACTION_STATE_BOOLEAN };
    getInfo.action = buttonXAction;
    xrGetActionStateBoolean(xrSession, &getInfo, &buttonXState);
    if (buttonXState.changedSinceLastSync) {
        if (buttonBState.currentState) {
            startEng.xrInput.OnButtonXPressed();
        }
        else {
            startEng.xrInput.OnButtonXReleased();
        }
    }

    //Y button
    XrActionStateBoolean buttonYState{ XR_TYPE_ACTION_STATE_BOOLEAN };
    getInfo.action = buttonYAction;
    xrGetActionStateBoolean(xrSession, &getInfo, &buttonYState);
    if (buttonYState.changedSinceLastSync) {
        if (buttonYState.currentState) {
            startEng.xrInput.OnButtonYPressed();
        }
        else {
            startEng.xrInput.OnButtonYReleased();
        }
    }


    //Menu button
    XrActionStateBoolean menuState{ XR_TYPE_ACTION_STATE_BOOLEAN };
    getInfo.action = menuAction;
    xrGetActionStateBoolean(xrSession, &getInfo, &menuState);
    if (menuState.changedSinceLastSync) {
        if (menuState.currentState) {
            startEng.xrInput.OnMenuPressed();
        }
        else {
            startEng.xrInput.OnMenuReleased();
        }
    }


    //Left Trigger float
    XrActionStateFloat leftTriggerState{ XR_TYPE_ACTION_STATE_FLOAT };
    getInfo.action = leftTriggerAction;
    xrGetActionStateFloat(xrSession, &getInfo, &leftTriggerState);
    if (leftTriggerState.changedSinceLastSync) {
        if (leftTriggerState.currentState > 0.0f) {
            startEng.xrInput.OnLeftTriggerPressed(leftTriggerState.currentState);
        }
        else {
            startEng.xrInput.OnLeftTriggerReleased();
        }
    }

    //Right Trigger float
    XrActionStateFloat rightTriggerState{ XR_TYPE_ACTION_STATE_FLOAT };
    getInfo.action = rightTriggerAction;
    xrGetActionStateFloat(xrSession, &getInfo, &rightTriggerState);
    if (rightTriggerState.changedSinceLastSync) {
        if (rightTriggerState.currentState > 0.0f) {
            startEng.xrInput.OnRightTriggerPressed(rightTriggerState.currentState);
        }
        else {
            startEng.xrInput.OnRightTriggerReleased();
        }
    }


    //Left Grip float
    XrActionStateFloat leftGripState{ XR_TYPE_ACTION_STATE_FLOAT };
    getInfo.action = leftGripAction;
    xrGetActionStateFloat(xrSession, &getInfo, &leftGripState);
    if (leftGripState.changedSinceLastSync) {
        if (leftGripState.currentState > 0.0f) {
            startEng.xrInput.OnLeftGripPressed(leftGripState.currentState);
        }
        else {
            startEng.xrInput.OnLeftGripReleased();
        }
    }

    //Right Grip float
    XrActionStateFloat rightGripState{ XR_TYPE_ACTION_STATE_FLOAT };
    getInfo.action = rightGripAction;
    xrGetActionStateFloat(xrSession, &getInfo, &rightGripState);
    if (rightGripState.changedSinceLastSync) {
        if (rightGripState.currentState > 0.0f) {
            startEng.xrInput.OnRightGripPressed(rightGripState.currentState);
        }
        else {
            startEng.xrInput.OnRightGripReleased();
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
        startEng.xrInput.OnLeftThumbstickMove(leftXThumbstickState.currentState, leftYThumbstickState.currentState);
    }

    //Right Thumbstick axes
    XrActionStateFloat rightXThumbstickState{ XR_TYPE_ACTION_STATE_FLOAT };
    getInfo.action = rightXThumbstickAction;
    xrGetActionStateFloat(xrSession, &getInfo, &rightXThumbstickState);

    XrActionStateFloat rightYThumbstickState{ XR_TYPE_ACTION_STATE_FLOAT };
    getInfo.action = rightYThumbstickAction;
    xrGetActionStateFloat(xrSession, &getInfo, &rightYThumbstickState);

    if (rightXThumbstickState.changedSinceLastSync || rightYThumbstickState.changedSinceLastSync) {
        startEng.xrInput.OnRightThumbstickMove(rightXThumbstickState.currentState, rightYThumbstickState.currentState);
    }
}



bool OpenXRSetup::SetupGraphics() {

    XrResult result;


    //Swap Chain
    XrSwapchainCreateInfo swapchainCreateInfo = {};
    swapchainCreateInfo.type = XR_TYPE_SWAPCHAIN_CREATE_INFO;
    swapchainCreateInfo.format = colorFormat; // or your preferred format
    swapchainCreateInfo.sampleCount = 1;
    swapchainCreateInfo.width = swapchainWidth;
    swapchainCreateInfo.height = swapchainHeight;
    swapchainCreateInfo.faceCount = 1;
    swapchainCreateInfo.arraySize = 1;
    swapchainCreateInfo.mipCount = 1;
    swapchainCreateInfo.createFlags = 0;
    swapchainCreateInfo.usageFlags = XR_SWAPCHAIN_USAGE_COLOR_ATTACHMENT_BIT |
        XR_SWAPCHAIN_USAGE_SAMPLED_BIT |
        XR_SWAPCHAIN_USAGE_TRANSFER_SRC_BIT |
        XR_SWAPCHAIN_USAGE_TRANSFER_DST_BIT;

    // Create swapchains for both eyes
    result = xrCreateSwapchain(xrSession, &swapchainCreateInfo, &xrLeftEyeSwapchain);
    if (XR_FAILED(result)) {
        throw std::runtime_error("Failed to attach action sets to session.");
        return false;
    }

    result = xrCreateSwapchain(xrSession, &swapchainCreateInfo, &xrRightEyeSwapchain);
    if (XR_FAILED(result)) {
        throw std::runtime_error("Failed to attach action sets to session.");
        return false;
    }


    

    return true;
}

bool OpenXRSetup::SetupXRLayers() {

    XrResult result;


    XrReferenceSpaceCreateInfo referenceSpaceCreateInfo = {};
    referenceSpaceCreateInfo.type = XR_TYPE_REFERENCE_SPACE_CREATE_INFO;
    referenceSpaceCreateInfo.referenceSpaceType = XR_REFERENCE_SPACE_TYPE_STAGE; // or XR_REFERENCE_SPACE_TYPE_LOCAL
    referenceSpaceCreateInfo.poseInReferenceSpace.orientation.w = 1.0f;
    referenceSpaceCreateInfo.poseInReferenceSpace.position = { 0.0f, 0.0f, 0.0f };

    result = xrCreateReferenceSpace(xrSession, &referenceSpaceCreateInfo, &referenceSpace);
    if (XR_FAILED(result)) {
        throw std::runtime_error("w");
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
    projectionViews[0].subImage.imageRect.extent.width = swapchainWidth;
    projectionViews[0].subImage.imageRect.extent.height = swapchainHeight;



    projectionViews[1].type = XR_TYPE_COMPOSITION_LAYER_PROJECTION_VIEW;
    projectionViews[1].pose = defaultPose;
    projectionViews[1].fov = fov;
    projectionViews[1].subImage.swapchain = xrRightEyeSwapchain;
    projectionViews[1].subImage.imageArrayIndex = 0;
    projectionViews[1].subImage.imageRect.offset.x = 0;
    projectionViews[1].subImage.imageRect.offset.y = 0;
    projectionViews[1].subImage.imageRect.extent.width = swapchainWidth;
    projectionViews[1].subImage.imageRect.extent.height = swapchainHeight;



    projectionLayer.views = projectionViews;
    projectionLayer.viewCount = 2;
}



void OpenXRSetup::StartFrame() {

    XrResult result;

    frameState = { XR_TYPE_FRAME_STATE };
    result = xrWaitFrame(xrSession, nullptr, &frameState);
    if (XR_FAILED(result)) {

        throw std::runtime_error("w");
    }

    //OpenXR
    XrFrameBeginInfo frameBeginInfo = {};
    frameBeginInfo.type = XR_TYPE_FRAME_BEGIN_INFO;
    result = xrBeginFrame(xrSession, &frameBeginInfo);
    if (XR_FAILED(result)) {

        throw std::runtime_error("w");
    }


    uint32_t imageIndex;

    XrSwapchainImageAcquireInfo leftEyeAcquireInfo = { XR_TYPE_SWAPCHAIN_IMAGE_ACQUIRE_INFO };
    XrSwapchainImageWaitInfo leftEyeWaitInfo = { XR_TYPE_SWAPCHAIN_IMAGE_WAIT_INFO, 0, XR_INFINITE_DURATION };

    result = xrAcquireSwapchainImage(xrLeftEyeSwapchain, &leftEyeAcquireInfo, &imageIndex);
    if (XR_FAILED(result)) {

        throw std::runtime_error("w");
    }
    result = xrWaitSwapchainImage(xrLeftEyeSwapchain, &leftEyeWaitInfo);
    if (XR_FAILED(result)) {

        throw std::runtime_error("w");
    }



    uint32_t swapchainImageCount;
    result = xrEnumerateSwapchainImages(xrLeftEyeSwapchain, 0, &swapchainImageCount, nullptr);
    if (XR_FAILED(result)) {

        throw std::runtime_error("w");
    }

    std::vector<XrSwapchainImageD3D11KHR> leftEyeSwapchainImageData(swapchainImageCount, { XR_TYPE_SWAPCHAIN_IMAGE_D3D11_KHR });

    result = xrEnumerateSwapchainImages(xrLeftEyeSwapchain, swapchainImageCount, &swapchainImageCount, reinterpret_cast<XrSwapchainImageBaseHeader*>(leftEyeSwapchainImageData.data()));
    if (XR_FAILED(result)) {

        throw std::runtime_error("w");
    }






    leftEyeTexture = leftEyeSwapchainImageData[imageIndex].texture;
    leftEyeRTV = nullptr;
    D3D11_RENDER_TARGET_VIEW_DESC rtvDesc = {};
    rtvDesc.Format = colorFormat;
    rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
    rtvDesc.Texture2D.MipSlice = 0;
    HRESULT hResult = startEng.D3DGfx().device->CreateRenderTargetView(leftEyeTexture, &rtvDesc, &leftEyeRTV);
    if (FAILED(hResult)) {

        throw std::runtime_error("w");
    }




    //Right
    XrSwapchainImageAcquireInfo rightEyeAcquireInfo = { XR_TYPE_SWAPCHAIN_IMAGE_ACQUIRE_INFO };
    XrSwapchainImageWaitInfo rightEyeWaitInfo = { XR_TYPE_SWAPCHAIN_IMAGE_WAIT_INFO, 0, XR_INFINITE_DURATION };

    result = xrAcquireSwapchainImage(xrRightEyeSwapchain, &rightEyeAcquireInfo, &imageIndex);
    if (XR_FAILED(result)) {

        throw std::runtime_error("w");
    }
    result = xrWaitSwapchainImage(xrRightEyeSwapchain, &rightEyeWaitInfo);
    if (XR_FAILED(result)) {

        throw std::runtime_error("w");
    }




    result = xrEnumerateSwapchainImages(xrRightEyeSwapchain, 0, &swapchainImageCount, nullptr);
    if (XR_FAILED(result)) {

        throw std::runtime_error("w");
    }

    std::vector<XrSwapchainImageD3D11KHR> rightEyeSwapchainImageData(swapchainImageCount, { XR_TYPE_SWAPCHAIN_IMAGE_D3D11_KHR });

    result = xrEnumerateSwapchainImages(xrRightEyeSwapchain, swapchainImageCount, &swapchainImageCount, reinterpret_cast<XrSwapchainImageBaseHeader*>(rightEyeSwapchainImageData.data()));
    if (XR_FAILED(result)) {

        throw std::runtime_error("w");
    }




    rightEyeTexture = rightEyeSwapchainImageData[imageIndex].texture;
    rightEyeRTV = nullptr;
    rtvDesc = {};
    rtvDesc.Format = colorFormat;
    rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
    rtvDesc.Texture2D.MipSlice = 0;
    hResult = startEng.D3DGfx().device->CreateRenderTargetView(rightEyeTexture, &rtvDesc, &rightEyeRTV);
    if (FAILED(hResult)) {

        throw std::runtime_error("w");
    }



    /*uint32_t viewCount;
    xrEnumerateViewConfigurationViews(xrInstance, systemId, XR_VIEW_CONFIGURATION_TYPE_PRIMARY_STEREO, 0, &viewCount, nullptr);

    std::vector<XrView> views(viewCount, { XR_TYPE_VIEW });
    XrViewLocateInfo viewLocateInfo{ XR_TYPE_VIEW_LOCATE_INFO };
    viewLocateInfo.space = referenceSpace;
    viewLocateInfo.displayTime = frameState.predictedDisplayTime;

    xrLocateViews(xrSession, &viewLocateInfo, &viewCount, views.data());*/

    SetXRRenderTargets();

    ClearBuffers();
}


void OpenXRSetup::SetXRRenderTargets() {


    rtvs[0] = leftEyeRTV;
    rtvs[1] = rightEyeRTV;
    rtvs[2] = startEng.D3DGfx().target;

    startEng.D3DGfx().deviceContext->OMSetRenderTargets(3, rtvs, startEng.D3DGfx().depthView);
}


void OpenXRSetup::ClearBuffers() {

    //Needs color correction...
    startEng.D3DGfx().ClearBuffer(0.2f, 0.2f, 0.7f, leftEyeRTV);
    startEng.D3DGfx().ClearBuffer(0.2f, 0.2f, 0.7f, rightEyeRTV);
}


void OpenXRSetup::EndFrame() {

    XrResult result;



    XrSwapchainImageReleaseInfo releaseInfo = { XR_TYPE_SWAPCHAIN_IMAGE_RELEASE_INFO };
    result = xrReleaseSwapchainImage(xrLeftEyeSwapchain, &releaseInfo);
    if (XR_FAILED(result)) {

        throw std::runtime_error("w");
    }

    result = xrReleaseSwapchainImage(xrRightEyeSwapchain, &releaseInfo);
    if (XR_FAILED(result)) {

        throw std::runtime_error("w");
    }



    XrTime predictedDisplayTime = frameState.predictedDisplayTime;//GetPredictedDisplayTime();



    XrCompositionLayerBaseHeader* layers[] = { reinterpret_cast<XrCompositionLayerBaseHeader*>(&projectionLayer) };


    XrFrameEndInfo frameEndInfo = {};
    frameEndInfo.type = XR_TYPE_FRAME_END_INFO;
    frameEndInfo.displayTime = predictedDisplayTime;
    frameEndInfo.environmentBlendMode = XR_ENVIRONMENT_BLEND_MODE_OPAQUE;
    frameEndInfo.layerCount = 1;
    frameEndInfo.layers = layers;

    result = xrEndFrame(xrSession, &frameEndInfo);
    if (XR_FAILED(result)) {

        throw std::runtime_error("w");
    }
}


void OpenXRSetup::ClearResources() {

    xrDestroySwapchain(xrLeftEyeSwapchain);
    xrDestroySwapchain(xrRightEyeSwapchain);
    xrEndSession(xrSession);
    xrDestroySession(xrSession);
    xrDestroyInstance(xrInstance);
}