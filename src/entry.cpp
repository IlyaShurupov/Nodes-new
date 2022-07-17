
#include "primitives/primitives.h"

#include "nodes.h"


void define_types();

int main() {

	obj::objects_init();
	obj::primitives_define_types();

	define_types();
	nd::operators_initialize();

	((nd::NodesCore*) obj::NDO->create("NodesCore"))->run();

	nd::operators_finalize();
	obj::objects_finalize();
}

void define_types() {

	obj::NDO->define(&nd::NodesCore::TypeData);
	obj::NDO->define(&nd::UI::TypeData);
	obj::NDO->define(&nd::GUI::TypeData);
	obj::NDO->define(&nd::TUI::TypeData);
	obj::NDO->define(&nd::Shortcut::TypeData);
	obj::NDO->define(&nd::KeyInput::TypeData);
	obj::NDO->define(&nd::Comparator::TypeData);
	obj::NDO->define(&nd::ShortcutCompareExpr::TypeData);
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
	obj::NDO->type_groups.addType(&nd::ShortcutCompareExpr::TypeData, {"Nodes", "UIs", "TUI"});
	obj::NDO->type_groups.addType(&nd::RootWidget::TypeData, {"Nodes", "UIs", "GUI"});
	obj::NDO->type_groups.addType(&nd::Requester::TypeData, {"Nodes", "Operators"});
}