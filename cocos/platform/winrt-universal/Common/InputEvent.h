/****************************************************************************
Copyright (c) 2013 cocos2d-x.org
Copyright (c) Microsoft Open Technologies, Inc.

http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/

#ifndef __APP_INPUT_EVENT__
#define __APP_INPUT_EVENT__

#include <agile.h>


namespace AngleApp
{
class Renderer;


enum PointerEventType
{
    PointerPressed,
    PointerMoved,
    PointerReleased,
};

enum KeyboardEventType
{
	KeyDown,
	KeyUp
};

class InputEvent
{
public:
    InputEvent() {};
    virtual ~InputEvent() {};
    virtual void execute(Renderer* renderer) = 0;
};

class PointerEvent : public InputEvent
{
public:
    PointerEvent(PointerEventType type, Windows::UI::Core::PointerEventArgs^ args);
    virtual void execute(Renderer* renderer);

private:
    PointerEventType m_type;
    Platform::Agile<Windows::UI::Core::PointerEventArgs> m_args;
};

class KeyboardEvent : public InputEvent
{
public:
    KeyboardEvent();
	KeyboardEvent(KeyboardEventType type, Windows::UI::Core::KeyEventArgs^ e);
    virtual void execute(Renderer* renderer);

private:
	Platform::Agile<Windows::UI::Core::KeyEventArgs> m_args;
	KeyboardEventType m_type;
};


}

#endif // #ifndef __APP_INPUT_EVENT__

