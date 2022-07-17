#pragma once

#include "primitives/primitives.h"

#include "rect.h"

namespace nd {

	struct GUI;

	struct Widget : obj::ClassObject {
		static obj::ObjectType TypeData;

		static void constructor(Widget* self) {
			self->createMember("int", "pos x");
			self->createMember("int", "pos y");
			self->createMember("int", "size x");
			self->createMember("int", "size y");

			self->createMember("dict", "child widgets");

			self->setRect(200, 100, 1500, 900);
		}

		static void destructor(Widget* self) {}
		static void copy(Widget* self, const Widget* in) {}

		static struct Vtable {
			void (*procInputs)(Widget*, GUI*) = NULL;
			void (*presentOutput)(Widget*, GUI*) = NULL;
		} vtable;

		void procInputs(GUI* gui) { ((Vtable*) (type->vtable))->procInputs(this, gui); };

		void presentOutput(GUI* gui, tp::rect<tp::alni> world_rec);


		void setRect(tp::alni x, tp::alni y, tp::alni w, tp::alni h) {
			getMember<obj::IntObject>("pos x")->val = x;
			getMember<obj::IntObject>("pos y")->val = y;
			getMember<obj::IntObject>("size x")->val = w;
			getMember<obj::IntObject>("size y")->val = h;
		}

		tp::rect<tp::alni> getRect() {
			return tp::rect<tp::alni> (
				getMember<obj::IntObject>("pos x")->val,
				getMember<obj::IntObject>("pos y")->val,
				getMember<obj::IntObject>("size x")->val,
				getMember<obj::IntObject>("size y")->val
			);
		}
	};


	struct RootWidget : Widget {

		static obj::ObjectType TypeData;
		static struct Vtable : Widget::Vtable {} vtable;

		static void constructor(Widget* self) {}
		static void destructor(Widget* self) {}
		static void copy(Widget* self, const Widget* in) {}


		static void procInputs(RootWidget* self, GUI* gui) {}
		static void presentOutput(RootWidget* self, GUI* gui);
	};
};