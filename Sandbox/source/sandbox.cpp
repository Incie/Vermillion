#include"pch/pch.h"
#include"sandbox.h"

Sandbox::Sandbox()
	: timer(0.0f), timerEnd(2.f), elapsed(0.0f), timePerThing(0.25f), goalText("VERMILLION ENGINE")
{
}

Sandbox::~Sandbox()
{
}

void Sandbox::ResetValues()
{
    timer = 0.0f;
    timerEnd = 2.f;
    elapsed = 0.0f;
    timePerThing = 0.25f;
    displayText = "";
}

void Sandbox::Initialize()
{
    TRACE("Sandbox");
    ResetValues();    
}

void Sandbox::Deinitialize()
{
    TRACE("Sandbox");
}

void Sandbox::Update(float delta)
{
/*
  if(!initGLEW(hInstance))return false;

   hWnd = a_hWnd;
   hDC = GetDC(*hWnd);

   bool bError = false;
   PIXELFORMATDESCRIPTOR pfd;

   if(iMajorVersion <= 2)
   {
      memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
            pfd.nSize= sizeof(PIXELFORMATDESCRIPTOR);
      pfd.nVersion   = 1;
      pfd.dwFlags    = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;
      pfd.iPixelType = PFD_TYPE_RGBA;
      pfd.cColorBits = 32;
      pfd.cDepthBits = 32;
      pfd.iLayerType = PFD_MAIN_PLANE;

      int iPixelFormat = ChoosePixelFormat(hDC, &pfd);
      if (iPixelFormat == 0)return false;

      if(!SetPixelFormat(hDC, iPixelFormat, &pfd))return false;

      // Create the old style context (OpenGL 2.1 and before)
      hRC = wglCreateContext(hDC);
      if(hRC)wglMakeCurrent(hDC, hRC);
      else bError = true;
   }
   else if(WGLEW_ARB_create_context && WGLEW_ARB_pixel_format)
   {
      const int iPixelFormatAttribList[] =
      {
         WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
         WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
         WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
         WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
         WGL_COLOR_BITS_ARB, 32,
         WGL_DEPTH_BITS_ARB, 24,
         WGL_STENCIL_BITS_ARB, 8,
         0 // End of attributes list
      };
      int iContextAttribs[] =
      {
         WGL_CONTEXT_MAJOR_VERSION_ARB, iMajorVersion,
         WGL_CONTEXT_MINOR_VERSION_ARB, iMinorVersion,
         WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,
         0 // End of attributes list
      };

      int iPixelFormat, iNumFormats;
      wglChoosePixelFormatARB(hDC, iPixelFormatAttribList, NULL, 1, &iPixelFormat, (UINT*)&iNumFormats);

      // PFD seems to be only redundant parameter now
      if(!SetPixelFormat(hDC, iPixelFormat, &pfd))return false;

      hRC = wglCreateContextAttribsARB(hDC, 0, iContextAttribs);
      // If everything went OK
      if(hRC) wglMakeCurrent(hDC, hRC);
      else bError = true;

   }
   else bError = true;

   if(bError)
   {
      // Generate error messages
      char sErrorMessage[255], sErrorTitle[255];
      sprintf(sErrorMessage, "OpenGL %d.%d is not supported! Please download latest GPU drivers!", iMajorVersion, iMinorVersion);
      sprintf(sErrorTitle, "OpenGL %d.%d Not Supported", iMajorVersion, iMinorVersion);
      MessageBox(*hWnd, sErrorMessage, sErrorTitle, MB_ICONINFORMATION);
      return false;
   }

   RenderScene = a_RenderScene;
   InitScene = a_InitScene;
   ReleaseScene = a_ReleaseScene;

   if(InitScene != NULL)InitScene(lpParam);

   return true;
*/

    elapsed += delta;

    if (timer < 0.0f)
        timer = 0.0f;

    timer += delta;

    if (timer >= timePerThing) {
        timer -= 0.0f;
        timePerThing -= 0.02f;
        glm::min(timePerThing, 0.01f);

        if( displayText.size() < goalText.size() )
            displayText.push_back(goalText[displayText.size()]);
    }


    if (Services().Input().KeyOnce(VKey_SPACE)) {
        displayText = "";
        timer = 0.0f;
        timePerThing = 0.25f;
        elapsed = 0.0f;
    }

    if(elapsed > timerEnd || Services().Input().KeyOnce(VKey_LBUTTON) ) {
        StartActivity("MenuTest");
    }
}

void Sandbox::Render()
{
	auto& text = Services().Text();
	const int textHeight = 26;

	auto width = text.CalculateWidth(displayText, textHeight);

    auto midX = WindowState::Width() * 0.5f - width * 0.5f;
	auto midY = WindowState::Height() * 0.5f - static_cast<float>(textHeight) * 0.5f;


	text.Print(midX, midY, displayText, textHeight, glm::vec3(1, 1, 1));
    text.Print(0, 25, fmt::format("{}", elapsed), 18, Colors::White);
}


