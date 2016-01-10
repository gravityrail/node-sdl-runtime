#include "render.h"
#include "SDL.h"
#include "helpers.h"
#include "texture.h"
#include "surface.h"
#include "struct_wrappers.h"
#include "window.h"
#include "container.h"

#include <iostream>

using namespace v8;

////////////////////////////////////////////////////////////////////////////////
// RendererWrapper Class Definition.
Nan::Persistent<FunctionTemplate> sdl::RendererWrapper::render_wrap_template_;
Nan::Persistent<FunctionTemplate> sdl::RendererWrapper::software_render_wrap_template_;

sdl::RendererWrapper::RendererWrapper() {
}

sdl::RendererWrapper::~RendererWrapper() {
	if(NULL != renderer_) {
		SDL_DestroyRenderer(renderer_);
	}
}

void sdl::RendererWrapper::Init(Handle<Object> exports) {
  	// Setup hardware renderer construction.
	Local<FunctionTemplate> tpl = FunctionTemplate::New(New);
	render_wrap_template_ = Persistent<FunctionTemplate>::New(tpl);

	render_wrap_template_->InstanceTemplate()->SetInternalFieldCount(1);
	render_wrap_template_->SetClassName(Nan::New<String>("RendererWrapper"));

	Nan::SetPrototypeMethod(render_wrap_template_, "getDrawBlendMode", GetDrawBlendMode);
	Nan::SetPrototypeMethod(render_wrap_template_, "getDrawColor", GetDrawColor);
	Nan::SetPrototypeMethod(render_wrap_template_, "getOutputSize", GetOutputSize);
	Nan::SetPrototypeMethod(render_wrap_template_, "getTarget", GetTarget);
	Nan::SetPrototypeMethod(render_wrap_template_, "getInfo", GetInfo);
	Nan::SetPrototypeMethod(render_wrap_template_, "getClipRect", GetClipRect);
	Nan::SetPrototypeMethod(render_wrap_template_, "getLogicalSize", GetLogicalSize);
	Nan::SetPrototypeMethod(render_wrap_template_, "getScale", GetScale);
	Nan::SetPrototypeMethod(render_wrap_template_, "getViewport", GetViewport);
	Nan::SetPrototypeMethod(render_wrap_template_, "targetSupported", TargetSupported);

	Nan::SetPrototypeMethod(render_wrap_template_, "setClipRect", SetClipRect);
	Nan::SetPrototypeMethod(render_wrap_template_, "setLogicalSize", SetLogicalSize);
	Nan::SetPrototypeMethod(render_wrap_template_, "setScale", SetScale);
	Nan::SetPrototypeMethod(render_wrap_template_, "setViewport", SetViewport);
	Nan::SetPrototypeMethod(render_wrap_template_, "setDrawBlendMode", SetDrawBlendMode);
	Nan::SetPrototypeMethod(render_wrap_template_, "setDrawColor", SetDrawColor);
	Nan::SetPrototypeMethod(render_wrap_template_, "setTarget", SetTarget);

	Nan::SetPrototypeMethod(render_wrap_template_, "clear", Clear);
	Nan::SetPrototypeMethod(render_wrap_template_, "present", Present);
	Nan::SetPrototypeMethod(render_wrap_template_, "copy", Copy);
	Nan::SetPrototypeMethod(render_wrap_template_, "drawLine", DrawLine);
	Nan::SetPrototypeMethod(render_wrap_template_, "drawLines", DrawLine);
	Nan::SetPrototypeMethod(render_wrap_template_, "drawPoint", DrawPoint);
	Nan::SetPrototypeMethod(render_wrap_template_, "drawRect", DrawRect);
	Nan::SetPrototypeMethod(render_wrap_template_, "fillRect", FillRect);

	Nan::Set(exports, Nan::New<String>("Renderer"), render_wrap_template_->GetFunction());

  // Setup software renderer construction.
	tpl = FunctionTemplate::New(NewSoftware);
	software_render_wrap_template_ = Persistent<FunctionTemplate>::New(tpl);

	software_render_wrap_template_->InstanceTemplate()->SetInternalFieldCount(1);
	software_render_wrap_template_->SetClassName(Nan::New<String>("SoftwareRendererWrapper"));

	Nan::SetPrototypeMethod(software_render_wrap_template_, "getDrawBlendMode", GetDrawBlendMode);
	Nan::SetPrototypeMethod(software_render_wrap_template_, "getDrawColor", GetDrawColor);
	Nan::SetPrototypeMethod(software_render_wrap_template_, "getOutputSize", GetOutputSize);
	Nan::SetPrototypeMethod(software_render_wrap_template_, "getTarget", GetTarget);
	Nan::SetPrototypeMethod(software_render_wrap_template_, "getInfo", GetInfo);
	Nan::SetPrototypeMethod(software_render_wrap_template_, "getClipRect", GetClipRect);
	Nan::SetPrototypeMethod(software_render_wrap_template_, "getLogicalSize", GetLogicalSize);
	Nan::SetPrototypeMethod(software_render_wrap_template_, "getScale", GetScale);
	Nan::SetPrototypeMethod(software_render_wrap_template_, "getViewport", GetViewport);
	Nan::SetPrototypeMethod(software_render_wrap_template_, "targetSupported", TargetSupported);

	Nan::SetPrototypeMethod(software_render_wrap_template_, "setClipRect", SetClipRect);
	Nan::SetPrototypeMethod(software_render_wrap_template_, "setLogicalSize", SetLogicalSize);
	Nan::SetPrototypeMethod(software_render_wrap_template_, "setScale", SetScale);
	Nan::SetPrototypeMethod(software_render_wrap_template_, "setViewport", SetViewport);
	Nan::SetPrototypeMethod(software_render_wrap_template_, "setDrawBlendMode", SetDrawBlendMode);
	Nan::SetPrototypeMethod(software_render_wrap_template_, "setDrawColor", SetDrawColor);
	Nan::SetPrototypeMethod(software_render_wrap_template_, "setTarget", SetTarget);

	Nan::SetPrototypeMethod(software_render_wrap_template_, "clear", Clear);
	Nan::SetPrototypeMethod(software_render_wrap_template_, "present", Present);
	Nan::SetPrototypeMethod(software_render_wrap_template_, "copy", Copy);
	Nan::SetPrototypeMethod(software_render_wrap_template_, "drawLine", DrawLine);
	Nan::SetPrototypeMethod(software_render_wrap_template_, "drawLines", DrawLine);
	Nan::SetPrototypeMethod(software_render_wrap_template_, "drawPoint", DrawPoint);
	Nan::SetPrototypeMethod(software_render_wrap_template_, "drawRect", DrawRect);
	Nan::SetPrototypeMethod(software_render_wrap_template_, "fillRect", FillRect);

	Nan::Set(exports, Nan::New<String>("SoftwareRenderer"), software_render_wrap_template_->GetFunction());
}

NAN_METHOD(sdl::RendererWrapper::New) {
	if(!info.IsConstructCall()) {
		return ThrowException(Exception::TypeError(
			Nan::New<String>("Use the new operator to create instances of a Renderer.")));
	}

	HandleScope scope;

	if(info.Length() < 1) {
		return ThrowException(Exception::TypeError(Nan::New<String>("Invalid Arguments: Expected at least: new Renderer(sdl.Window)")));
	}
	else if(!info[0]->IsObject()) {
		return ThrowException(Exception::TypeError(Nan::New<String>("Invalid Arguments: Expected at least: new Renderer(sdl.Window)")));
	}

	WindowWrapper* window = Nan::ObjectWrap::Unwrap<WindowWrapper>(Handle<Object>::Cast(info[0]));
	int index = info[1]->IsUndefined() ? -1 : info[1]->Int32Value();
	int flags = info[2]->IsUndefined() ? SDL_RENDERER_ACCELERATED : info[2]->Int32Value();
	SDL_Renderer* renderer = SDL_CreateRenderer(window->window_, index, flags);
	if(NULL == renderer) {
		return ThrowSDLException(__func__);
	}

	RendererWrapper* obj = new RendererWrapper();
	obj->renderer_ = renderer;
	obj->Wrap(info.This());
	return info.This();
}

NAN_METHOD(sdl::RendererWrapper::NewSoftware) {
	if(!info.IsConstructCall()) {
		return ThrowException(Exception::TypeError(
			Nan::New<String>("Use the new operator to create instances of a Renderer.")));
	}

	HandleScope scope;

	if(info.Length() < 1) {
		return ThrowException(Exception::TypeError(Nan::New<String>("Invalid Arguments: Expected: new SoftwareRenderer(sdl.Surface)")));
	}
	else if(!info[0]->IsObject()) {
		return ThrowException(Exception::TypeError(Nan::New<String>("Invalid Arguments: Expected: new SoftwareRenderer(sdl.Surface)")));
	}

	SurfaceWrapper* wrap = Nan::ObjectWrap::Unwrap<SurfaceWrapper>(Handle<Object>::Cast(info[0]));
	SDL_Renderer* renderer = SDL_CreateSoftwareRenderer(wrap->surface_);
	if(NULL == renderer) {
		return ThrowSDLException(__func__);
	}

	RendererWrapper* obj = new RendererWrapper();
	obj->renderer_ = renderer;
	obj->Wrap(info.This());
	return info.This();
}

// NAN_METHOD(sdl::RendererWrapper::CreateTexture) {
//   HandleScope scope;
//   RendererWrapper* obj = Nan::ObjectWrap::Unwrap<RendererWrapper>(info.This());

//   return Undefined();
// }
// NAN_METHOD(sdl::RendererWrapper::CreateTextureFromSurface) {
//   HandleScope scope;
//   RendererWrapper* obj = Nan::ObjectWrap::Unwrap<RendererWrapper>(info.This());

//   return Undefined();
// }

NAN_METHOD(sdl::RendererWrapper::GetDrawBlendMode) {
	HandleScope scope;
	RendererWrapper* obj = Nan::ObjectWrap::Unwrap<sdl::RendererWrapper>(info.This());
	SDL_BlendMode mode;
	int err = SDL_GetRenderDrawBlendMode(obj->renderer_, &mode);
	if(err < 0) {
		return ThrowSDLException(__func__);
	}

	return scope.Close(Nan::New<Number>(mode));
}

NAN_METHOD(sdl::RendererWrapper::GetDrawColor) {
	HandleScope scope;
	RendererWrapper* obj = Nan::ObjectWrap::Unwrap<RendererWrapper>(info.This());
	uint8_t r, g, b, a;
	int err = SDL_GetRenderDrawColor(obj->renderer_, &r, &g, &b, &a);
	if(err < 0) {
		return ThrowSDLException(__func__);
	}
	SDL_Color* color = new SDL_Color;
	color->r = r;
	color->g = g;
	color->b = b;
	color->a = a;

	return scope.Close(WrapColor(color));
}

NAN_METHOD(sdl::RendererWrapper::GetTarget) {
	HandleScope scope;
	RendererWrapper* obj = Nan::ObjectWrap::Unwrap<RendererWrapper>(info.This());

	SDL_Texture* texture = SDL_GetRenderTarget(obj->renderer_);
	if(NULL == texture) {
		return Null();
	}

	Handle<Object> toWrap = Nan::New<Object>();
	TextureWrapper* texWrap = new TextureWrapper(toWrap);
	texWrap->texture_ = texture;
	return scope.Close(toWrap);
}

NAN_METHOD(sdl::RendererWrapper::GetInfo) {
	HandleScope scope;
	RendererWrapper* obj = Nan::ObjectWrap::Unwrap<RendererWrapper>(info.This());

	SDL_RendererInfo* info = new SDL_RendererInfo;
	int err = SDL_GetRendererInfo(obj->renderer_, info);
	if(err < 0) {
		return ThrowSDLException(__func__);
	}
	return scope.Close(WrapRendererInfo(info));
}

NAN_METHOD(sdl::RendererWrapper::GetOutputSize) {
	HandleScope scope;
	RendererWrapper* obj = Nan::ObjectWrap::Unwrap<sdl::RendererWrapper>(info.This());

	int w, h;
	int err = SDL_GetRendererOutputSize(obj->renderer_, &w, &h);
	if(err < 0) {
		return ThrowSDLException(__func__);
	}
	Local<Array> ret = Array::New(2);
	Nan::Set(ret, 0, Nan::New<Number>(w));
	Nan::Set(ret, 1, Nan::New<Number>(h));
	return scope.Close(ret);
}

NAN_METHOD(sdl::RendererWrapper::GetClipRect) {
	HandleScope scope;
	RendererWrapper* obj = Nan::ObjectWrap::Unwrap<RendererWrapper>(info.This());

	SDL_Rect* rect = new SDL_Rect;
	SDL_RenderGetClipRect(obj->renderer_, rect);

  NEW_WRAPPED(rect, RectWrapper, ret)

	return scope.Close(ret);
}

NAN_METHOD(sdl::RendererWrapper::GetLogicalSize) {
	HandleScope scope;
	RendererWrapper* obj = Nan::ObjectWrap::Unwrap<RendererWrapper>(info.This());

	int w, h;
	SDL_RenderGetLogicalSize(obj->renderer_, &w, &h);
	Handle<Array> ret = Array::New(2);
	Nan::Set(ret, 0, Nan::New<Number>(w));
	Nan::Set(ret, 1, Nan::New<Number>(h));

	return scope.Close(ret);
}
NAN_METHOD(sdl::RendererWrapper::GetScale) {
	HandleScope scope;
	RendererWrapper* obj = Nan::ObjectWrap::Unwrap<RendererWrapper>(info.This());

	float scaleX, scaleY;
	SDL_RenderGetScale(obj->renderer_, &scaleX, &scaleY);
	Handle<Array> ret = Array::New(2);
	Nan::Set(ret, 0, Nan::New<Number>(scaleX));
	Nan::Set(ret, 1, Nan::New<Number>(scaleY));

	return scope.Close(ret);
}
NAN_METHOD(sdl::RendererWrapper::GetViewport) {
	HandleScope scope;
	RendererWrapper* obj = Nan::ObjectWrap::Unwrap<RendererWrapper>(info.This());

	SDL_Rect* rect = new SDL_Rect;
	SDL_RenderGetViewport(obj->renderer_, rect);

  NEW_WRAPPED(rect, RectWrapper, ret)

	return scope.Close(ret);
}
// TODO: Implement.
NAN_METHOD(sdl::RendererWrapper::ReadPixels) {
	HandleScope scope;
	// RendererWrapper* obj = Nan::ObjectWrap::Unwrap<RendererWrapper>(info.This());

	return ThrowException(Exception::Error(Nan::New<String>("Not implemented.")));
}
NAN_METHOD(sdl::RendererWrapper::TargetSupported) {
	HandleScope scope;
	RendererWrapper* obj = Nan::ObjectWrap::Unwrap<RendererWrapper>(info.This());

	return scope.Close(Nan::New<Boolean>(SDL_RenderTargetSupported(obj->renderer_)));
}

NAN_METHOD(sdl::RendererWrapper::SetClipRect) {
	HandleScope scope;
	RendererWrapper* obj = Nan::ObjectWrap::Unwrap<RendererWrapper>(info.This());

	int err;
	if(info[0]->IsUndefined()) {
		err = SDL_RenderSetClipRect(obj->renderer_, NULL);
	}
	else {
		RectWrapper* clip = Nan::ObjectWrap::Unwrap<RectWrapper>(Handle<Object>::Cast(info[0]));
		err = SDL_RenderSetClipRect(obj->renderer_, clip->wrapped);
	}
	if(err < 0) {
		return ThrowSDLException(__func__);
	}

	return Undefined();
}
NAN_METHOD(sdl::RendererWrapper::SetLogicalSize) {
	HandleScope scope;
	RendererWrapper* obj = Nan::ObjectWrap::Unwrap<RendererWrapper>(info.This());

	if(info.Length() < 2) {
		return ThrowException(Exception::TypeError(Nan::New<String>("Invalid arguments: Expected setLogicalSize(Number, Number)")));
	}
	int w = info[0]->IsUndefined() ? 1 : info[0]->Int32Value();
	int h = info[1]->IsUndefined() ? 1 : info[1]->Int32Value();
	int err = SDL_RenderSetLogicalSize(obj->renderer_, w, h);
	if(err < 0) {
		return ThrowSDLException(__func__);
	}

	return Undefined();
}
NAN_METHOD(sdl::RendererWrapper::SetScale) {
	HandleScope scope;
	RendererWrapper* obj = Nan::ObjectWrap::Unwrap<RendererWrapper>(info.This());

	if(info.Length() < 2) {
		return ThrowException(Exception::TypeError(Nan::New<String>("Invalid arguments: Expected setLogicalSize(Number, Number)")));
	}
	double scaleX = info[0]->IsUndefined() ? 1 : info[0]->NumberValue();
	double scaleY = info[1]->IsUndefined() ? 1 : info[1]->NumberValue();
	int err = SDL_RenderSetLogicalSize(obj->renderer_, scaleX, scaleY);
	if(err < 0) {
		return ThrowSDLException(__func__);
	}

	return Undefined();
}
NAN_METHOD(sdl::RendererWrapper::SetViewport) {
	HandleScope scope;
	RendererWrapper* obj = Nan::ObjectWrap::Unwrap<RendererWrapper>(info.This());

	int err;
	if(info[0]->IsUndefined()) {
		err = SDL_RenderSetViewport(obj->renderer_, NULL);
	}
	else {
		RectWrapper* wrap = Nan::ObjectWrap::Unwrap<RectWrapper>(Handle<Object>::Cast(info[0]));
		err = SDL_RenderSetViewport(obj->renderer_, wrap->wrapped);
	}
	if(err < 0) {
		return ThrowSDLException(__func__);
	}

	return Undefined();
}
NAN_METHOD(sdl::RendererWrapper::SetDrawBlendMode) {
	HandleScope scope;
	RendererWrapper* obj = Nan::ObjectWrap::Unwrap<RendererWrapper>(info.This());

	SDL_BlendMode mode = info[0]->IsUndefined() ? SDL_BLENDMODE_NONE : static_cast<SDL_BlendMode>(info[0]->Int32Value());
	int err = SDL_SetRenderDrawBlendMode(obj->renderer_, mode);
	if(err < 0) {
		return ThrowSDLException(__func__);
	}

	return Undefined();
}
NAN_METHOD(sdl::RendererWrapper::SetDrawColor) {
	HandleScope scope;
	RendererWrapper* obj = Nan::ObjectWrap::Unwrap<RendererWrapper>(info.This());

	if(info.Length() < 4) {
		return ThrowException(Exception::TypeError(Nan::New<String>("Invalid arguments: Expected setDrawColor(Number, Number, Number, Number)")));
	}
	int r = info[0]->IsUndefined() ? 0 : info[0]->Int32Value();
	int g = info[1]->IsUndefined() ? 0 : info[1]->Int32Value();
	int b = info[2]->IsUndefined() ? 0 : info[2]->Int32Value();
	int a = info[3]->IsUndefined() ? 0 : info[4]->Int32Value();
	int err = SDL_SetRenderDrawColor(obj->renderer_, r, g, b, a);
	if(err < 0) {
		return ThrowSDLException(__func__);
	}

	return Undefined();
}
NAN_METHOD(sdl::RendererWrapper::SetTarget) {
	HandleScope scope;
	RendererWrapper* obj = Nan::ObjectWrap::Unwrap<RendererWrapper>(info.This());

	if(info.Length() < 1) {
		return ThrowException(Exception::TypeError(Nan::New<String>("Invalid arguments: Expected setTarget(Texture)")));
	}
	TextureWrapper* tex = Nan::ObjectWrap::Unwrap<TextureWrapper>(Handle<Object>::Cast(info[0]));
	int err = SDL_SetRenderTarget(obj->renderer_, tex->texture_);
	if(err < 0) {
		return ThrowSDLException(__func__);
	}

	return Undefined();
}

NAN_METHOD(sdl::RendererWrapper::Clear) {
	HandleScope scope;
	RendererWrapper* obj = Nan::ObjectWrap::Unwrap<RendererWrapper>(info.This());

	int err = SDL_RenderClear(obj->renderer_);
	if(err < 0) {
		return ThrowSDLException(__func__);
	}

	return Undefined();
}
NAN_METHOD(sdl::RendererWrapper::Present) {
	HandleScope scope;
	RendererWrapper* obj = Nan::ObjectWrap::Unwrap<RendererWrapper>(info.This());
	SDL_RenderPresent(obj->renderer_);

	return Undefined();
}
NAN_METHOD(sdl::RendererWrapper::Copy) {
	HandleScope scope;
	RendererWrapper* obj = Nan::ObjectWrap::Unwrap<RendererWrapper>(info.This());

	if(info.Length() > 3) {
		TextureWrapper* texture = Nan::ObjectWrap::Unwrap<TextureWrapper>(Handle<Object>::Cast(info[0]));
		RectWrapper* src = info[1]->IsUndefined() ? NULL : Nan::ObjectWrap::Unwrap<RectWrapper>(Handle<Object>::Cast(info[1]));
		RectWrapper* dst = info[2]->IsUndefined() ? NULL : Nan::ObjectWrap::Unwrap<RectWrapper>(Handle<Object>::Cast(info[2]));
		double angle = info[3]->IsUndefined() ? 0 : info[3]->NumberValue();
		PointWrapper* point = info[4]->IsUndefined() ? NULL : Nan::ObjectWrap::Unwrap<PointWrapper>(Handle<Object>::Cast(info[4]));
		SDL_RendererFlip flip = info[5]->IsUndefined() ? SDL_FLIP_NONE : static_cast<SDL_RendererFlip>(info[5]->Int32Value());
		int err = SDL_RenderCopyEx(obj->renderer_, texture->texture_,
			src == NULL ? NULL : src->wrapped,
			dst == NULL ? NULL : dst->wrapped,
			angle,
			point == NULL ? NULL : point->point_,
			flip);
		if(err < 0) {
			return ThrowSDLException(__func__);
		}
	}
	else {
		TextureWrapper* texture = Nan::ObjectWrap::Unwrap<TextureWrapper>(Handle<Object>::Cast(info[0]));
		RectWrapper* src = info[1]->IsUndefined() ? NULL : Nan::ObjectWrap::Unwrap<RectWrapper>(Handle<Object>::Cast(info[1]));
		RectWrapper* dst = info[2]->IsUndefined() ? NULL : Nan::ObjectWrap::Unwrap<RectWrapper>(Handle<Object>::Cast(info[2]));
		int err = SDL_RenderCopy(obj->renderer_, texture->texture_,
			src == NULL ? NULL : src->wrapped,
			dst == NULL ? NULL : dst->wrapped);
		if(err < 0) {
			return ThrowSDLException(__func__);
		}
	}

	return Undefined();
}
NAN_METHOD(sdl::RendererWrapper::DrawLine) {
	HandleScope scope;
	RendererWrapper* obj = Nan::ObjectWrap::Unwrap<RendererWrapper>(info.This());

	if(info.Length() < 4) {
		return ThrowException(Exception::TypeError(Nan::New<String>("Invalid arguments: Expected drawLine(Number, Number, Number, Number)")));
	}

	int x1 = info[0]->Int32Value();
	int y1 = info[1]->Int32Value();
	int x2 = info[2]->Int32Value();
	int y2 = info[3]->Int32Value();
	int err = SDL_RenderDrawLine(obj->renderer_, x1, y1, x2, y2);
	if(err < 0) {
		return ThrowSDLException(__func__);
	}

	return Undefined();
}
NAN_METHOD(sdl::RendererWrapper::DrawLines) {
	HandleScope scope;
	RendererWrapper* obj = Nan::ObjectWrap::Unwrap<RendererWrapper>(info.This());

	if(info.Length() < 1) {
		return ThrowException(Exception::TypeError(Nan::New<String>("Invalid arguments: Expected drawLines(Array)")));
	}

	Handle<Array> arr = Handle<Array>::Cast(info[0]);
	const int numPoints = arr->Length();
	SDL_Point* points = new SDL_Point[numPoints];
	for(int i = 0; i < numPoints; i++) {
		PointWrapper* wrap = Nan::ObjectWrap::Unwrap<PointWrapper>(Handle<Object>::Cast(arr->Get(0)));
		points[i] = *wrap->point_;
	}
	int err = SDL_RenderDrawLines(obj->renderer_, points, numPoints);
	delete points;
	if(err < 0) {
		return ThrowSDLException(__func__);
	}

	return Undefined();
}
NAN_METHOD(sdl::RendererWrapper::DrawPoint) {
	HandleScope scope;
	RendererWrapper* obj = Nan::ObjectWrap::Unwrap<RendererWrapper>(info.This());

	if(info.Length() < 2) {
		return ThrowException(Exception::TypeError(Nan::New<String>("Invalid arguments: Expected drawPoint(Point)")));
	}

	int x = info[0]->Int32Value();
	int y = info[1]->Int32Value();
	int err = SDL_RenderDrawPoint(obj->renderer_, x, y);
	if(err < 0) {
		return ThrowSDLException(__func__);
	}

	return Undefined();
}
NAN_METHOD(sdl::RendererWrapper::DrawPoints) {
	HandleScope scope;
	RendererWrapper* obj = Nan::ObjectWrap::Unwrap<RendererWrapper>(info.This());

	if(info.Length() < 1) {
		return ThrowException(Exception::TypeError(Nan::New<String>("Invalid arguments: Expected drawPoints(Array)")));
	}

	Handle<Array> arr = Handle<Array>::Cast(info[0]);
	int numPoints = arr->Length();
	SDL_Point* points = new SDL_Point[numPoints];
	for(int i = 0; i < numPoints; i++) {
		PointWrapper* point = Nan::ObjectWrap::Unwrap<PointWrapper>(Handle<Object>::Cast(arr->Get(i)));
		points[i] = *point->point_;
	}
	int err = SDL_RenderDrawPoints(obj->renderer_, points, numPoints);
	delete points;
	if(err < 0) {
		return ThrowSDLException(__func__);
	}

	return Undefined();
}
NAN_METHOD(sdl::RendererWrapper::DrawRect) {
	HandleScope scope;
	RendererWrapper* obj = Nan::ObjectWrap::Unwrap<RendererWrapper>(info.This());

	if(info.Length() < 1) {
		return ThrowException(Exception::TypeError(Nan::New<String>("Invalid arguments: Expected drawRect(Rect)")));
	}

	RectWrapper* rect = Nan::ObjectWrap::Unwrap<RectWrapper>(Handle<Object>::Cast(info[0]));
	int err = SDL_RenderDrawRect(obj->renderer_, rect->wrapped);
	if(err < 0) {
		return ThrowSDLException(__func__);
	}

	return Undefined();
}
NAN_METHOD(sdl::RendererWrapper::DrawRects) {
	HandleScope scope;
	RendererWrapper* obj = Nan::ObjectWrap::Unwrap<RendererWrapper>(info.This());

	if(info.Length() < 1) {
		return ThrowException(Exception::TypeError(Nan::New<String>("Invalid arguments: Expected drawRects(Array)")));
	}

	Handle<Array> arr = Handle<Array>::Cast(info[0]);
	int numRects = arr->Length();
	SDL_Rect* rects = new SDL_Rect[numRects];
	for(int i = 0; i < numRects; i++) {
		RectWrapper* rect = Nan::ObjectWrap::Unwrap<RectWrapper>(Handle<Object>::Cast(arr->Get(i)));
		rects[i] = *rect->wrapped;
	}
	int err = SDL_RenderDrawRects(obj->renderer_, rects, numRects);
	delete rects;
	if(err < 0) {
		return ThrowSDLException(__func__);
	}

	return Undefined();
}
NAN_METHOD(sdl::RendererWrapper::FillRect) {
	HandleScope scope;
	RendererWrapper* obj = Nan::ObjectWrap::Unwrap<RendererWrapper>(info.This());

	if(info.Length() < 1) {
		return ThrowException(Exception::TypeError(Nan::New<String>("Invalid arguments: Expected drawRect(Rect)")));
	}

	RectWrapper* rect = Nan::ObjectWrap::Unwrap<RectWrapper>(Handle<Object>::Cast(info[0]));
	int err = SDL_RenderFillRect(obj->renderer_, rect->wrapped);
	if(err < 0) {
		return ThrowSDLException(__func__);
	}

	return Undefined();
}
NAN_METHOD(sdl::RendererWrapper::FillRects) {
	HandleScope scope;
	RendererWrapper* obj = Nan::ObjectWrap::Unwrap<RendererWrapper>(info.This());

	if(info.Length() < 1) {
		return ThrowException(Exception::TypeError(Nan::New<String>("Invalid arguments: Expected drawRects(Array)")));
	}

	Handle<Array> arr = Handle<Array>::Cast(info[0]);
	int numRects = arr->Length();
	SDL_Rect* rects = new SDL_Rect[numRects];
	for(int i = 0; i < numRects; i++) {
		RectWrapper* rect = Nan::ObjectWrap::Unwrap<RectWrapper>(Handle<Object>::Cast(arr->Get(i)));
		rects[i] = *rect->wrapped;
	}
	int err = SDL_RenderFillRects(obj->renderer_, rects, numRects);
	delete rects;
	if(err < 0) {
		return ThrowSDLException(__func__);
	}

	return Undefined();
}
