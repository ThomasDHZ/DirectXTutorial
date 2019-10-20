#include "pch.h"
#include "Game.h"

using namespace Windows::ApplicationModel;
using namespace Windows::ApplicationModel::Core;
using namespace Windows::ApplicationModel::Activation;
using namespace Windows::UI::Core;
using namespace Windows::UI::Popups;
using namespace Windows::System;
using namespace Windows::Foundation;
using namespace Windows::Graphics::Display;
using namespace Platform;

ref class HelloWorld sealed : public IFrameworkView
{
private:
	bool CloseWindow;
	Game game;
public:
	virtual void Initialize(CoreApplicationView^ appView)
	{
		CloseWindow = false;

		appView->Activated += ref new Windows::Foundation::TypedEventHandler<Windows::ApplicationModel::Core::CoreApplicationView^, Windows::ApplicationModel::Activation::IActivatedEventArgs^>(this, &HelloWorld::OnActivated);
		
		CoreApplication::Suspending += ref new Windows::Foundation::EventHandler<Windows::ApplicationModel::SuspendingEventArgs^>(this, &HelloWorld::OnSuspending);
		CoreApplication::Resuming += ref new Windows::Foundation::EventHandler<Platform::Object^>(this, &HelloWorld::OnResuming);
	}
	virtual void SetWindow(CoreWindow^ Window)
	{
		Window->PointerPressed += ref new Windows::Foundation::TypedEventHandler<Windows::UI::Core::CoreWindow^, Windows::UI::Core::PointerEventArgs^>(this, &HelloWorld::OnPointerPressed);
		Window->PointerWheelChanged += ref new Windows::Foundation::TypedEventHandler<Windows::UI::Core::CoreWindow^, Windows::UI::Core::PointerEventArgs^>(this, &HelloWorld::OnPointerWheelChanged);
		Window->KeyDown += ref new Windows::Foundation::TypedEventHandler<Windows::UI::Core::CoreWindow^, Windows::UI::Core::KeyEventArgs^>(this, &HelloWorld::OnKeyDown);
		Window->Closed += ref new Windows::Foundation::TypedEventHandler<Windows::UI::Core::CoreWindow^, Windows::UI::Core::CoreWindowEventArgs^>(this, &HelloWorld::OnClosed);
	}

	virtual void Load(String^ EntryPoint) {}

	virtual void Run() 
	{
		CoreWindow^ Window = CoreWindow::GetForCurrentThread();
		game.Initialize();

		while (!CloseWindow)
		{
			game.Update();
			game.Render();

			Window->Dispatcher->ProcessEvents(CoreProcessEventsOption::ProcessUntilQuit);
		}
	}

	virtual void Uninitialize() {}

	void OnActivated(Windows::ApplicationModel::Core::CoreApplicationView^ sender, Windows::ApplicationModel::Activation::IActivatedEventArgs^ args)
	{
		CoreWindow^ Window = CoreWindow::GetForCurrentThread();
		Window->Activate();
	}

	void OnSuspending(Platform::Object^ sender, Windows::ApplicationModel::SuspendingEventArgs^ args)
	{
	}

	void OnResuming(Platform::Object^ sender, Platform::Object^ args)
	{
	}

	void OnPointerPressed(CoreWindow^ window, PointerEventArgs^ Args)
	{
		MessageDialog Dialog(Args->CurrentPoint->RawPosition.X + " " + Args->CurrentPoint->RawPosition.Y, "Hey Listen");
		Dialog.ShowAsync();
	}

	void OnPointerWheelChanged(CoreWindow^ window, PointerEventArgs^ Args)
	{
		int wheel = Args->CurrentPoint->Properties->MouseWheelDelta;

		MessageDialog Dialog(wheel.ToString(), "Hey Listen");
		Dialog.ShowAsync();
	}

	void OnKeyDown(CoreWindow^ window, KeyEventArgs^ Args)
	{
		if (Args->VirtualKey == VirtualKey::W)
		{
			MessageDialog Dialog("Space Pressed", "Hey Listen");
			Dialog.ShowAsync();
		}
	}

	void OnClosed(Windows::UI::Core::CoreWindow^ sender, Windows::UI::Core::CoreWindowEventArgs^ args)
	{
		CloseWindow = true;
	}
};

ref class AppSource sealed : IFrameworkViewSource
{
private:
public:
	virtual IFrameworkView^ CreateView()
	{
		return ref new HelloWorld();
	}
};

[MTAThreadAttribute]
int main(Array<String^>^ args)
{
	CoreApplication::Run(ref new AppSource());
	return 0;
}

