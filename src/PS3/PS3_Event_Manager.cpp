#include "PS3_Event_Manager.h"
#include "Utilities.h"
#include <vector>
#include <sysutil/sysutil_common.h>
#include <sysutil/sysutil_sysparam.h>
#include "PS3_Input_handler.h"
#include "GameEngine.h"

namespace Engine{
	namespace PS3{
			struct ps3SytemEvent{
				unsigned long long status;
				unsigned long long param;
			};

			std::vector<ps3SytemEvent> eventQueue;

			static void sysutil_callback(unsigned long long status, unsigned long long  param, void *userdata)
			{
				ps3SytemEvent e;
				e.param = param;
				e.status = status;
				eventQueue.push_back(e);

			}

			void PS3EventManager::processEvents(){
				if (eventQueue.size() > 0)
				{
					for (std::vector<ps3SytemEvent>::iterator it = eventQueue.begin(); it != eventQueue.end(); ++it) {
						printf("System Event! %#08x\n", it->status);
						if (it->status == CELL_SYSUTIL_REQUEST_EXITGAME) {
							printf("System has requested EXITGAME\n");
							GameEngine::run = false;
						}
					}
					eventQueue.clear();
				}
				//poll the controllers
				PS3_Input::Update();
			}

			void PS3EventManager::init(){
				int err = cellSysutilRegisterCallback(0, sysutil_callback, NULL);
				DBG_ASSERT_MSG(err == 0, "cellSysutilRegisterCallback failed !");

				PS3_Input::Initialise();
			}
	}
}