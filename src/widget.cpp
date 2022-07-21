
#include "widget.h"

#include "gui.h"

#define NANOVG_GL3_IMPLEMENTATION	// Use GL3 implementation.
#include "nanovg.h"

using namespace nd;

struct obj::ObjectType Widget::TypeData = {
	.base = &obj::ClassObject::TypeData,
	.constructor = (obj::object_constructor) Widget::constructor,
	.destructor = (obj::object_destructor) Widget::destructor,
	.copy = (obj::object_copy) Widget::copy,
	.size = sizeof(Widget),
	.name = "Widget",
	.vtable = &Widget::vtable
};

struct Widget::Vtable Widget::vtable = {NULL, NULL};

void nd::Widget::presentOutput(GUI* gui, tp::rect<tp::alni> world_rec) {

	nvgScissor(gui->vg, (float) world_rec.x, (float) world_rec.y, (float) world_rec.z, (float) world_rec.w);

	((Vtable*) (type->vtable))->presentOutput(this, gui);

	nvgResetScissor(gui->vg);

	obj::DictObject* childs = getMember<obj::DictObject>("child widgets");

	for (auto& iter : childs->items) {
		Widget* widget = NDO_CAST(Widget, iter.iter->val);
		if (widget) {

			tp::rect<tp::alni> widget_rec = widget->getRect();
			world_rec.x += widget_rec.x;
			world_rec.y += widget_rec.y;

			CLAMP(widget_rec.z, 0, world_rec.z - widget_rec.x);
			CLAMP(widget_rec.w, 0, world_rec.w - widget_rec.y);

			world_rec.z = widget_rec.z;
			world_rec.w = widget_rec.w;

			widget->presentOutput(gui, world_rec);
		}
	}

};

 // ------------  RootWidget  ------------ //

struct obj::ObjectType RootWidget::TypeData = {
	.base = &Widget::TypeData,
	.constructor = (obj::object_constructor) RootWidget::constructor,
	.destructor = (obj::object_destructor) RootWidget::destructor,
	.copy = (obj::object_copy) RootWidget::copy,
	.size = sizeof(RootWidget),
	.name = "RootWidget",
	.vtable = &RootWidget::vtable
};

struct RootWidget::Vtable RootWidget::vtable = {
	(void (*)(Widget*, GUI*)) & RootWidget::procInputs,
	(void (*)(Widget*, GUI*)) & RootWidget::presentOutput,
};

void RootWidget::procInputs(RootWidget* self, GUI* gui) {
	//tp::rect<tp::alni> rec = self->getRect();
}

void RootWidget::presentOutput(RootWidget* self, GUI* gui) {
	tp::rect<tp::alni> rec = self->getRect();

	nvgBeginPath(gui->vg);

	//nvgPathWinding(gui->vg, NVG_SOLID);	// Mark circle as a hole.
	nvgRoundedRect(gui->vg, (float) rec.x, (float) rec.y, (float) rec.z, (float) rec.w, 10);
	//nvgPathWinding(gui->vg, NVG_HOLE);	// Mark circle as a hole.
	//nvgCircle(gui->vg, 120, 120, 25);

	//nvgRect(vg, 50, 50, 220, 130);

	nvgFillColor(gui->vg, nvgRGBA(20, 20, 20, 250));
	nvgFill(gui->vg);
}
