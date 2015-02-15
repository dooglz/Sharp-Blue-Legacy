#include <Rocket/Core.h>
#include <Rocket/Controls.h>
#include <Rocket/Debugger/Debugger.h>
#include "LibRocketInterface.h"
#include "LibRocketSystemInterface.h"
#include "LibRocketRenderInterface.h"
#include "../Storage.h"

//----------- Event Bouncer ---------------------------
class EventBouncer : public Rocket::Core::EventListener {
public:
  Rocket::Core::ElementDocument* _parentDocument;
  const std::string _type;
  const std::string _elementID;
  void (*Gamefunc)(const std::string& elementID);

  EventBouncer(Rocket::Core::ElementDocument* parentDocument,
               const std::string& type, const std::string& elementID,
               void (*ListenerFunction)(const std::string& elementID));
  ~EventBouncer();
  void ProcessEvent(Rocket::Core::Event& event) {
    Gamefunc(event.GetTargetElement()->GetId().CString());
  }
};

EventBouncer::EventBouncer(
    Rocket::Core::ElementDocument* parentDocument, const std::string& type,
    const std::string& elementID,
    void (*ListenerFunction)(const std::string& elementID))
    : _type(type),
      _elementID(elementID),
      Gamefunc(ListenerFunction),
      _parentDocument(parentDocument)
{
  _parentDocument->GetElementById(elementID.c_str())->AddEventListener(_type.c_str(), this, false);
};

EventBouncer::~EventBouncer() {
_parentDocument->GetElementById(_elementID.c_str())->RemoveEventListener(_type.c_str(), this, false);
}
//----------- /Event Bouncer ---------------------------

namespace Engine {
UIDocument::UIDocument() { internalPointer = 0; }

UIDocument::~UIDocument() {
  RemoveAllListeners();
  Rocket::Core::ElementDocument* d = ((Rocket::Core::ElementDocument*)internalPointer);
  d->Close();
}

void UIDocument::SetContent(const std::string& elementID,
                            const std::string& content) {
  ((Rocket::Core::ElementDocument*)internalPointer)
      ->GetElementById(elementID.c_str())
      ->SetInnerRML(content.c_str());
}

std::string UIDocument::GetContent(const std::string& elementID) {
  return ((Rocket::Core::ElementDocument*)internalPointer)
      ->GetElementById(elementID.c_str())
      ->GetInnerRML()
      .CString();
}

void UIDocument::AddEventListener(
    const std::string& elementID, const std::string& eventType,
    void (*ListenerFunction)(const std::string& elementID)) {

  for each(EventBouncer * b in _bouncers) {
      if (b->_elementID == elementID && b->_type == eventType &&
          b->Gamefunc == ListenerFunction) {
        printf("ERROR: Identical [%s] Event Listner already bound to "
               "element[%s]\n",
               eventType.c_str(), elementID.c_str());
        return;
      }
  }
  _bouncers.push_back(
      new EventBouncer(((Rocket::Core::ElementDocument*)internalPointer),
                       eventType, elementID, ListenerFunction));
}

void UIDocument::RemoveEventListener(
    const std::string& elementID, const std::string& eventType,
    void (*ListenerFunction)(const std::string& elementID)) {
  bool found = 0;
  for each(EventBouncer * b in _bouncers) {
      if (b->_elementID == elementID && b->_type == eventType &&
          b->Gamefunc == ListenerFunction) {
        found = 1;
        _bouncers.erase(std::remove(_bouncers.begin(), _bouncers.end(), b),
                        _bouncers.end());
        delete b;
        break;
      }
  }
  if (!found) {
    printf(
        "ERROR: Removing a non-existant [%s] listner bound to element [%s]\n",
        eventType.c_str(), elementID.c_str());
  }
}

void UIDocument::RemoveAllListeners()
{
  if (_bouncers.empty()) return;
  std::vector< EventBouncer*>::iterator itt = _bouncers.begin();
  while (itt != _bouncers.end()) {
    delete *itt;
    ++itt;
  }
  _bouncers.clear();
}

CUserInterface* UserInterface = 0;

UICanvas::UICanvas(unsigned int posX, unsigned int posY, unsigned int sizeX,
                   unsigned int sizeY)
    : posX(posX), posY(posY), sizeX(sizeX), sizeY(sizeY) {
  internalPointer =0;
}


UICanvas::~UICanvas()
{
  Unload();
 ((CLibrocket*)UserInterface)->RemoveCanvas(this);
}


UIDocument* UICanvas::LoadDocument(const std::string& name) {
  Rocket::Core::ElementDocument* document =
      ((Rocket::Core::Context*)internalPointer)->LoadDocument(name.c_str());
  if (document != NULL) {
    document->Show();
    document->RemoveReference();
    //document->GetElementById("button1")->SetInnerRML("We set this element with code!");
    //document->GetElementById("button1")->AddEventListener("click", el, false);
    // document->GetElementById("button1")->RemoveEventListener()
  } else {
    ASSERT(false);
  }
  UIDocument* doc = new UIDocument();
  doc->internalPointer = document;
  _documents.push_back(doc);
  return doc;
}

void UICanvas::Unload() {
  for each(UIDocument* d in _documents) {
    delete d;
  }
  _documents.clear();
  ((Rocket::Core::Context*)internalPointer)->UnloadAllDocuments();
}

CLibrocket::CLibrocket() {
  _uii=0;
  _uir=0;
  printf("Librocket constructed\n");
}
CLibrocket::~CLibrocket() {
  printf("Librocket destructed\n");
}

void CLibrocket::Init() {
  printf("Librocket version: %s\n\n", Rocket::Core::GetVersion().CString());

  _uii = new CLibRocketInterface();
  Rocket::Core::SetSystemInterface(_uii);
  _uir = new CLibRocketRenderInterface();
  Rocket::Core::SetRenderInterface(_uir);

  Rocket::Core::Initialise();
  Rocket::Controls::Initialise();

  Rocket::Core::FontDatabase::LoadFontFace("ui/assets/Delicious-Roman.otf");
  Rocket::Core::FontDatabase::LoadFontFace("ui/assets/Delicious-Italic.otf");
  Rocket::Core::FontDatabase::LoadFontFace(
      "ui/assets/Delicious-BoldItalic.otf");
  Rocket::Core::FontDatabase::LoadFontFace("ui/assets/Delicious-Bold.otf");
}

void CLibrocket::Update(const double delta) {
  for each(Rocket::Core::Context * context in contexts) { context->Update(); }
}
void CLibrocket::Render() {
  for each(Rocket::Core::Context * context in contexts) { context->Render(); }
}

void CLibrocket::Shutdown() {
  printf("Librocket shutting down\n");
  for each(Rocket::Core::Context * context in contexts) {
      context->UnloadAllDocuments();
      context->RemoveReference();
    }
  Rocket::Core::Shutdown();
  delete _uii;
  delete _uir;
}

void CLibrocket::ProcessKeyDown(SDL_Keycode sdlkey) {
  Rocket::Core::Input::KeyIdentifier key =
      CLibRocketInterface::TranslateKey(sdlkey);
  for each(Rocket::Core::Context * context in contexts) {
      context->ProcessKeyDown(key, 0);
    }
}
void CLibrocket::ProcessMouseWheel(int delta) {
  for each(Rocket::Core::Context * context in contexts) {
      context->ProcessMouseWheel(delta, 0);
    }
}
void CLibrocket::ProcessMouseMove(int x, int y) {
  for each(Rocket::Core::Context * context in contexts) {
      context->ProcessMouseMove(x, y, 0);
    }
}

void CLibrocket::ProcessMouseButtonDown(Uint8 sdlbutton) {
  int button = CLibRocketInterface::TranslateMouseButton(sdlbutton);
  for each(Rocket::Core::Context * context in contexts) {
      context->ProcessMouseButtonDown(button, 0);
    }
}

void CLibrocket::ProcessMouseButtonUp(Uint8 sdlbutton) {
  int button = CLibRocketInterface::TranslateMouseButton(sdlbutton);
  for each(Rocket::Core::Context * context in contexts) {
      context->ProcessMouseButtonUp(button, 0);
    }
}


UICanvas* CLibrocket::NewCanvas(const unsigned int posX,
                                const unsigned int posY,
                                const unsigned int sizeX,
                                const unsigned int sizeY,
                                const std::string& name) {

  Rocket::Core::Context* Rcontext = Rocket::Core::CreateContext(
      name.c_str(), Rocket::Core::Vector2i(sizeX, sizeY));
  Rocket::Debugger::SetVisible(true);

  // Todo: URGENT Only once
  // Rocket::Debugger::Initialise(Rcontext);
  // Rocket::Debugger::SetVisible(true);
  UICanvas* uic = new UICanvas(posX, posY, sizeX, sizeY);
  uic->internalPointer = Rcontext;
  contexts.push_back(Rcontext);
  return uic;
}

void CLibrocket::RemoveCanvas(UICanvas* canvas) {

  Rocket::Core::Context* c = ((Rocket::Core::Context*)canvas->internalPointer);
  c->RemoveReference();
  contexts.erase(std::remove(contexts.begin(), contexts.end(), (Rocket::Core::Context*)canvas->internalPointer), contexts.end());
}
}