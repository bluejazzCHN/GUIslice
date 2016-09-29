//
// microSDL GUI Library Examples
// - Calvin Hass
// - http://www.impulseadventure.com/elec/microsdl-sdl-gui.html
// - Example 04: Dynamic content
//

#include "microsdl.h"


// Defines for resources
#define FONT_DROID_SANS "/usr/share/fonts/truetype/droid/DroidSans.ttf"

// Enumerations for pages, elements, fonts, images
enum {E_PG_MAIN};
enum {E_ELEM_BOX,E_ELEM_BTN_QUIT,E_ELEM_TXT_COUNT,E_ELEM_PROGRESS};
enum {E_FONT_BTN,E_FONT_TXT};

// Free-running counter for display
unsigned m_nCount = 0;

// Main GUI instance
microSDL_tsGui  m_gui;

// Create page elements
bool InitOverlays()
{
  microSDL_tsElem   sElem;

  // Background flat color
  microSDL_SetBkgndColor(m_gui,m_colGrayDk);

  // Create background box
  sElem = microSDL_ElemCreateBox(m_gui,E_ELEM_BOX,E_PG_MAIN,(SDL_Rect){10,50,300,150});
  microSDL_ElemSetCol(m_gui,sElem.nId,m_colWhite,m_colBlack,m_colBlack);

  // Create Quit button with text label
  sElem = microSDL_ElemCreateBtnTxt(m_gui,E_ELEM_BTN_QUIT,E_PG_MAIN,
    (SDL_Rect){120,100,80,40},"Quit",E_FONT_BTN);

  // Create counter
  sElem = microSDL_ElemCreateTxt(m_gui,MSDL_ID_ANON,E_PG_MAIN,(SDL_Rect){20,60,50,10},
    "Count:",E_FONT_TXT);
  sElem = microSDL_ElemCreateTxt(m_gui,E_ELEM_TXT_COUNT,E_PG_MAIN,(SDL_Rect){80,60,50,10},
    "",E_FONT_TXT);
  microSDL_ElemSetTxtCol(m_gui,sElem.nId,m_colYellow);

  // Create progress bar
  sElem = microSDL_ElemCreateTxt(m_gui,MSDL_ID_ANON,E_PG_MAIN,(SDL_Rect){20,80,50,10},
    "Progress:",E_FONT_TXT);
  sElem = microSDL_ElemCreateGauge(m_gui,E_ELEM_PROGRESS,E_PG_MAIN,(SDL_Rect){80,80,50,10},
    0,100,0,m_colGreenDk,false);

  return true;
}


int main( int argc, char* args[] )
{
  bool              bOk = true;
  bool              bQuit = false;  
  int               nRet;
  int               nClickX,nClickY;
  unsigned          nClickPress;
  int               nElemId;
  int               nTrackElemClicked;

  char              acTxt[100];

  // -----------------------------------
  // Initialize

  microSDL_InitEnv(m_gui);
  microSDL_Init(m_gui);

  microSDL_InitFont(m_gui);
  microSDL_InitTs(m_gui,"/dev/input/touchscreen");

  // Load Fonts
  bOk = microSDL_FontAdd(m_gui,E_FONT_BTN,FONT_DROID_SANS,12);
  if (!bOk) { fprintf(stderr,"ERROR: FontAdd failed\n"); return false; }
  bOk = microSDL_FontAdd(m_gui,E_FONT_TXT,FONT_DROID_SANS,8);
  if (!bOk) { fprintf(stderr,"ERROR: FontAdd failed\n"); return false; }


  // -----------------------------------
  // Start display
  InitOverlays();

  // Start up display on main page
  microSDL_SetPageCur(m_gui,E_PG_MAIN);
  microSDL_ElemDrawPageCur(m_gui);

  // -----------------------------------
  // Main event loop

  bQuit = false;
  while (!bQuit) {

    m_nCount++;

    // Immediate update of element on active page
    sprintf(acTxt,"%u",m_nCount);
    microSDL_ElemSetTxtStr(m_gui,E_ELEM_TXT_COUNT,acTxt);
    microSDL_ElemDraw(m_gui,E_ELEM_TXT_COUNT);

    microSDL_ElemUpdateGauge(m_gui,E_ELEM_PROGRESS,((m_nCount/200)%100));
    microSDL_ElemDraw(m_gui,E_ELEM_PROGRESS); 
  
    // Poll for touchscreen presses
    if (microSDL_GetTsClick(m_gui,nClickX,nClickY,nClickPress)) {
 
      // Track the touch event and find any associated object
      microSDL_TrackClick(m_gui,nClickX,nClickY,nClickPress);
      nTrackElemClicked = microSDL_GetTrackElemClicked(m_gui);

      // Any selectable object clicked?
      if (nTrackElemClicked != MSDL_IND_NONE) {

        // Convert element index to element ID
        nElemId = microSDL_ElemGetIdFromInd(m_gui,nTrackElemClicked);
        if (nElemId == E_ELEM_BTN_QUIT) {
          // Quit button pressed
          bQuit = true;
        }
  
        // Clear click event
        microSDL_ClearTrackElemClicked(m_gui);
  
      } // Object clicked
    } // Touchscreen press
  } // bQuit


  // -----------------------------------
  // Close down display

  microSDL_FontCloseAll(m_gui);
  microSDL_Quit(m_gui);
}

