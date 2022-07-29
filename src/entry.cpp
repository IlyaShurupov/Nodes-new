
#include "primitives/primitives.h"

#include "nodes.h"


void define_types();

int main(int argc, char* argv[]) {

	tp::alloc_init();

	{
		obj::objects_init();
		obj::primitives_define_types();

		define_types();
		nd::operators_initialize();

		nd::NodesCore* nodes = NULL;
		tp::string nodes_path;
		bool new_instance = argc == 1;

		if (new_instance) {
			nodes = (nd::NodesCore*) obj::NDO->create("NodesCore");
		} else {
			if (argc == 2) {
				nodes_path = argv[1];
				obj::Object* nodes_object = obj::NDO->load(nodes_path);
				if (!nodes_object) {
					printf("invalid object file given");
				} else {
					nodes = NDO_CAST(nd::NodesCore, nodes_object);
					if (!nodes) {
						printf("loaded object is not NodesCore");
					}
				}
			} else {
				printf("invalid argument passed");
			}
		}

		if (nodes) {
			nodes->run();

			if (!nd::terminate_code && !new_instance) {
				obj::NDO->save(nodes, nodes_path);
			}
		}

		nd::operators_finalize();
		obj::objects_finalize();
	}

	tp::alloc_uninit();
}

void define_types() {

	obj::NDO->define(&nd::NodesCore::TypeData);
	obj::NDO->define(&nd::UI::TypeData);
	obj::NDO->define(&nd::GUI::TypeData);
	obj::NDO->define(&nd::TUI::TypeData);
	obj::NDO->define(&nd::Shortcut::TypeData);
	obj::NDO->define(&nd::KeyInput::TypeData);
	obj::NDO->define(&nd::Comparator::TypeData);
	obj::NDO->define(&nd::ShortcutTrigger::TypeData);
	obj::NDO->define(&nd::Widget::TypeData);
	obj::NDO->define(&nd::RootWidget::TypeData);
	obj::NDO->define(&nd::Requester::TypeData);

	obj::NDO->type_groups.addType(&nd::NodesCore::TypeData, {"Nodes"});
	obj::NDO->type_groups.addType(&nd::UI::TypeData, {"Nodes", "UIs"});
	obj::NDO->type_groups.addType(&nd::GUI::TypeData, {"Nodes", "UIs", "GUI"});
	obj::NDO->type_groups.addType(&nd::TUI::TypeData, {"Nodes", "UIs", "TUI"});
	obj::NDO->type_groups.addType(&nd::Widget::TypeData, {"Nodes", "UIs", "GUI"});
	obj::NDO->type_groups.addType(&nd::Shortcut::TypeData, {"Nodes", "UIs", "TUI"});
	obj::NDO->type_groups.addType(&nd::KeyInput::TypeData, {"Nodes", "UIs", "TUI"});
	obj::NDO->type_groups.addType(&nd::Comparator::TypeData, {"Nodes", "UIs", "TUI"});
	obj::NDO->type_groups.addType(&nd::ShortcutTrigger::TypeData, {"Nodes", "UIs", "TUI"});
	obj::NDO->type_groups.addType(&nd::RootWidget::TypeData, {"Nodes", "UIs", "GUI"});
	obj::NDO->type_groups.addType(&nd::Requester::TypeData, {"Nodes", "Operators"});
}

#ifdef PLATFORM_WINDOWS 

#include <windows.h>

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {
	int pNumArgs;
	LPWSTR* w_args = CommandLineToArgvW(LPCWSTR(lpCmdLine), &pNumArgs);
	char* args[2] = {0, lpCmdLine};
	return main(pNumArgs + 1, args);
}

#endif