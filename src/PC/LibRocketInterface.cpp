#include <Rocket/Core.h>
#include <Rocket/Controls.h>
#include <Rocket/Debugger/Debugger.h>
#include "LibRocketInterface.h"
#include "LibRocketSystemInterface.h"
#include "LibRocketRenderInterface.h"
#include "../Storage.h"

class EventBouncer : public Rocket::Core::EventListener {
private:
public:
  static EventBouncer* Load(const std::string& name) { return 0; }
  void (*Gamefunc)(const std::string& elementID);

  void ProcessEvent(Rocket::Core::Event& event) {
    Gamefunc(event.GetTargetElement()->GetId().CString());
  }
};

namespace Engine {
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

  if (Storage<EventBouncer>::Contains(elementID + eventType)) {
    printf(
        "ERROR: Identical [%s] Event Listner already bound to element [%s]\n",
        eventType.c_str(), elementID.c_str());
    return;
  }

  EventBouncer* bounce = new EventBouncer();
  bool b = Storage<EventBouncer>::Store(elementID + eventType, bounce);

  ((Rocket::Core::ElementDocument*)internalPointer)
      ->GetElementById(elementID.c_str())
      ->AddEventListener(eventType.c_str(), bounce, false);
  bounce->Gamefunc = ListenerFunction;
}

void UIDocument::RemoveEventListener(
    const std::string& elementID, const std::string& eventType,
    void (*ListenerFunction)(const std::string& elementID)) {
  //can we add &this  to the key, so we can have multiple docs?
  EventBouncer* bounce = Storage<EventBouncer>::Get(elementID + eventType);
  if (bounce == NULL) {
    printf(
        "ERROR: Removing a non-existant [%s] listner bound to element [%s]\n",
        eventType.c_str(), elementID.c_str());
    return;
  }

  ((Rocket::Core::ElementDocument*)internalPointer)
      ->GetElementById(elementID.c_str())
      ->RemoveEventListener(eventType.c_str(), bounce, false);

  // TODO: URGENT
  // Storage<EventBouncer>::Remove()
}

CUserInterface* UserInterface = 0;

UICanvas::UICanvas(unsigned int posX, unsigned int posY, unsigned int sizeX,
                   unsigned int sizeY)
    : posX(posX), posY(posY), sizeX(sizeX), sizeY(sizeY) {}

//""
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
  return doc;
}

void UICanvas::Unload() {
  //TODO: remove all even bouners

  ((Rocket::Core::Context*)internalPointer)->UnloadAllDocuments();
}

CLibrocket::CLibrocket() {}
CLibrocket::~CLibrocket() {}

void CLibrocket::Init() {
  printf("Librocket version: %s\n\n", Rocket::Core::GetVersion());

  CLibRocketInterface* uii = new CLibRocketInterface();
  Rocket::Core::SetSystemInterface(uii);
  CLibRocketRenderInterface* uir = new CLibRocketRenderInterface();
  Rocket::Core::SetRenderInterface(uir);

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
  for each(Rocket::Core::Context * context in contexts) {
      context->UnloadAllDocuments();
      context->RemoveReference();
    }
  Rocket::Core::Shutdown();
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
  contexts.push_back(Rcontext);
  uic->internalPointer = Rcontext;
  return uic;
}

void CLibrocket::RemoveCanvas(UICanvas* canvas) {
  ((Rocket::Core::Context*)canvas->internalPointer)->RemoveReference();
  // contexts.erase(std::remove(contexts.begin(), contexts.end(), canvas),
  // contexts.end());
  delete canvas;
  canvas = 0;
}
}