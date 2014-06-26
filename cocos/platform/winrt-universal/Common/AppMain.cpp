﻿#include "pch.h"
#include "AppMain.h"
#include "AngleHelper.h"

using namespace Windows::Foundation;
using namespace Windows::System::Threading;
using namespace Windows::UI::Core;
using namespace Concurrency;

namespace AngleApp
{

// Loads and initializes application assets when the application is loaded.
AppMain::AppMain(const std::shared_ptr<AngleApp::DeviceResources>& deviceResources) 
    : m_deviceResources(deviceResources)
    , m_tracking(false)
{
	// Register to be notified if the Device is lost or recreated
	m_deviceResources->RegisterDeviceNotify(this);

	// TODO: Replace this with your app's content initialization.
	m_renderer = std::unique_ptr<Renderer>(new Renderer(m_deviceResources));

	// TODO: Change the timer settings if you want something other than the default variable timestep mode.
	// e.g. for 60 FPS fixed timestep update logic, call:
	/*
	m_timer.SetFixedTimeStep(true);
	m_timer.SetTargetElapsedSeconds(1.0 / 60);
	*/
}

AppMain::~AppMain()
{
	// Deregister device notification
	m_deviceResources->RegisterDeviceNotify(nullptr);
}

// Updates application state when the window size changes (e.g. device orientation change)
void AppMain::CreateWindowSizeDependentResources() 
{
	// TODO: Replace this with the size-dependent initialization of your app's content.
	m_renderer->CreateWindowSizeDependentResources();
}

void AppMain::ofNotifyAppResume(int state) 
{
    critical_section::scoped_lock lock(m_criticalSection);
#if 0
    static ofAppResumeEventArgs entryArgs;
    entryArgs.state = state;
    ofNotifyEvent(ofEvents().appResume, entryArgs);
#endif // 0


}

void AppMain::ofNotifyAppSuspend(int state) 
{
    critical_section::scoped_lock lock(m_criticalSection);
#if 0
    static ofAppSuspendEventArgs entryArgs;
    entryArgs.state = state;
    ofNotifyEvent(ofEvents().appSuspend, entryArgs);
#endif

}

void AppMain::StartRenderLoop()
{
	// If the animation render loop is already running then do not start another thread.
	if (m_renderLoopWorker != nullptr && m_renderLoopWorker->Status == AsyncStatus::Started)
	{
		return;
	}

	// Create a task that will be run on a background thread.
	auto workItemHandler = ref new WorkItemHandler([this](IAsyncAction ^ action)
	{
        ofNotifyAppResume();
		// Calculate the updated frame and render once per vertical blanking interval.
		while (action->Status == AsyncStatus::Started)
		{
			critical_section::scoped_lock lock(m_criticalSection);
			Update();
            Render();

		}
	});

	// Run task on a dedicated high priority background thread.
	m_renderLoopWorker = ThreadPool::RunAsync(workItemHandler, WorkItemPriority::High, WorkItemOptions::TimeSliced);
}

void AppMain::StopRenderLoop()
{
    ofNotifyAppSuspend();
    m_renderLoopWorker->Cancel();
}

// Updates the application state once per frame.
void AppMain::Update() 
{
	// Update scene objects.
	m_timer.Tick([&]()
	{
		// TODO: Replace this with your app's content update functions.
		m_renderer->Update(m_timer);
	});
}



// Renders the current frame according to the current application state.
// Returns true if the frame was rendered and is ready to be displayed.
bool AppMain::Render() 
{
	// Don't try to render anything before the first Update.
	if (m_timer.GetFrameCount() == 0)
	{
		return false;
	}

	// Render the scene objects.
	// TODO: Replace this with your app's content rendering functions.
	m_renderer->Render();

	return true;
}

// Notifies renderers that device resources need to be released.
void AppMain::OnDeviceLost()
{
	m_renderer->ReleaseDeviceDependentResources();
}

// Notifies renderers that device resources may now be recreated.
void AppMain::OnDeviceRestored()
{
	//m_renderer->CreateDeviceDependentResources();
	//CreateWindowSizeDependentResources();
}

void AppMain::OnKeyPressed(Windows::UI::Core::KeyEventArgs^ e)
{
	m_renderer->AddKeyboardEvent(KeyboardEventType::KeyDown, e);

}

void AppMain::OnKeyReleased(Windows::UI::Core::KeyEventArgs^ e)
{
	m_renderer->AddKeyboardEvent(KeyboardEventType::KeyUp, e);
}


void AppMain::OnPointerPressed(PointerEventArgs^ e)
{
    m_tracking = true;
    m_renderer->AddPointerEvent(PointerEventType::PointerPressed, e);
}

void AppMain::OnPointerMoved(PointerEventArgs^ e)
{
    if (IsTracking())
    {
        m_renderer->AddPointerEvent(PointerEventType::PointerMoved, e);
    }
}

void AppMain::OnPointerReleased(PointerEventArgs^ e)
{
    m_tracking = false;
    m_renderer->AddPointerEvent(PointerEventType::PointerReleased, e);
}



}


