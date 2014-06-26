#include "pch.h"
#include "Renderer.h"
#include "AngleHelper.h"
#include "cocos2d.h"
#include "CCApplication.h"
#include "CCGLView.h"
#include "AppDelegate.h"

using namespace DirectX;
using namespace Windows::Foundation;
using namespace Windows::UI::Core;
USING_NS_CC;


namespace AngleApp
{


// Loads vertex and pixel shaders from files and instantiates the cube geometry.
Renderer::Renderer(const std::shared_ptr<AngleApp::DeviceResources>& deviceResources) 
    : m_loadingComplete(false)
    , m_setupComplete(false)
    , m_deviceResources(deviceResources)
{
    mApp = new AppDelegate();
    CreateDeviceDependentResources();
}

// Initializes view parameters when the window size changes.
void Renderer::CreateWindowSizeDependentResources()
{
    m_deviceResources->aquireContext();
    auto outputSize = m_deviceResources->GetOutputSize();
    auto logicalSize = m_deviceResources->GetLogicalSize();
    auto director = cocos2d::Director::getInstance();


    if (!m_setupComplete)
    {
        GLView* glview = GLView::create("Test Cpp");
        glview->Create(logicalSize.Width, logicalSize.Height, m_deviceResources->GetCurrentOrientation());
        director->setOpenGLView(glview);
        CCApplication::getInstance()->run();
    }
    else
    {
        cocos2d::GL::invalidateStateCache();
        cocos2d::ShaderCache::getInstance()->reloadDefaultGLPrograms();
        cocos2d::DrawPrimitives::init();
        cocos2d::VolatileTextureMgr::reloadAllTextures();
        cocos2d::EventCustom foregroundEvent(EVENT_COME_TO_FOREGROUND);
        director->getEventDispatcher()->dispatchEvent(&foregroundEvent);
        cocos2d::Application::getInstance()->applicationWillEnterForeground();
        director->setGLDefaultValues();
    }
    m_setupComplete = true;
    m_deviceResources->releaseContext();

}

// Called once per frame, rotates the cube and calculates the model and view matrices.
void Renderer::Update(AngleApp::StepTimer const& timer)
{

}

void Renderer::OnPointerPressed(PointerEventArgs^ e)
{
    //window->OnPointerPressed(e);
}

void Renderer::OnPointerMoved(PointerEventArgs^ e)
{
    //window->OnPointerMoved(e);
}

void Renderer::OnPointerReleased(PointerEventArgs^ e)
{
    //window->OnPointerReleased(e);
}

void Renderer::OnKeyPressed(Windows::UI::Core::KeyEventArgs^ e)
{
	//window->OnKeyPressed(e);
}

void Renderer::OnKeyReleased(Windows::UI::Core::KeyEventArgs^ e)
{
	//window->OnKeyReleased(e);
}



void Renderer::AddPointerEvent(PointerEventType type, PointerEventArgs^ args)
{
    std::lock_guard<std::mutex> guard(mMutex);
	std::shared_ptr<PointerEvent> e(new PointerEvent(type, args));
    mInputEvents.push(e);
}

void Renderer::AddKeyboardEvent(KeyboardEventType type, Windows::UI::Core::KeyEventArgs^ args)
{
	std::lock_guard<std::mutex> guard(mMutex);
	std::shared_ptr<KeyboardEvent> e(new KeyboardEvent(type, args));
	mInputEvents.push(e);
}



void Renderer::ProcessEvents()
{
    std::lock_guard<std::mutex> guard(mMutex);

    while (!mInputEvents.empty())
    {
        InputEvent* e = mInputEvents.front().get();
        e->execute(this);
        mInputEvents.pop();
    }
}


// Renders one frame using the vertex and pixel shaders.
void Renderer::Render()
{
    if (m_setupComplete)
	{
        m_deviceResources->aquireContext();

        ProcessEvents();
#if 0
        glClearColor(1.0, 0.0, 0.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);
#else
        GLView* glview = GLView::sharedOpenGLView();
        glview->Render();

#endif
        m_deviceResources->Present();

        m_deviceResources->releaseContext();
    }
}

void Renderer::CreateDeviceDependentResources()
{

}

void Renderer::ReleaseDeviceDependentResources()
{
	m_loadingComplete = false;
}

}