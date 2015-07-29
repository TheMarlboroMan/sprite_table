#ifndef FRAME_H
#define FRAME_H

#include <iostream>
#include <vector>
#include <algorithm>
#include <memory>

#include <libDan2.h>

//namespace Aplicacion
//{

class Frame;

std::ostream& operator<<(std::ostream& stream, const Frame& f);
std::istream& operator>>(std::istream& stream, Frame& f);

class Frame
{
	private:

	unsigned int id, x, y, w, h;
	int desp_x, desp_y;

	std::unique_ptr<DLibV::Representacion_texto_auto_dinamica> rep_txt_id;
	std::unique_ptr<DLibV::Representacion_primitiva_caja_dinamica> rep_caja;

	public:

	Frame():
		id(0), x(0), y(0), w(0), h(0),
		desp_x(0), desp_y(0), rep_txt_id(nullptr), rep_caja(nullptr)
	{}

	Frame(unsigned int pid, unsigned int px, unsigned int py, unsigned int pw, unsigned int ph, int dx, int dy):
		id(pid), x(px), y(py), w(pw), h(ph), desp_x(dx), desp_y(dy), rep_txt_id(nullptr), rep_caja(nullptr)
	{}

	Frame(const Frame&)=delete;
	Frame& operator=(const Frame&)=delete;

	Frame(Frame&& f):
		id(f.id), x(f.x), y(f.y), w(f.w), h(f.h),
		desp_x(f.desp_x), desp_y(f.desp_y), 
		rep_txt_id(std::move(f.rep_txt_id)), 
		rep_caja(std::move(f.rep_caja))
	{}

	Frame& operator=(Frame&& f)
	{
		id=f.id;
		x=f.x;
		y=f.y;
		w=f.w;
		h=f.h;
		desp_x=f.desp_x;
		desp_y=f.desp_y;
		rep_txt_id=std::move(f.rep_txt_id);
		rep_caja=std::move(f.rep_caja);
		return *this;
	}

	unsigned int acc_id() const {return id;}
	unsigned int acc_x() const {return x;}
	unsigned int acc_y() const {return y;}
	unsigned int acc_w() const {return w;}
	unsigned int acc_h() const {return h;}
	int acc_desp_x() const {return desp_x;}
	int acc_desp_y() const {return desp_y;}
	DLibV::Representacion_texto_auto_dinamica& acc_txt() {return *rep_txt_id;}
	DLibV::Representacion_primitiva_caja_dinamica& acc_caja() {return *rep_caja;}

	void mut_id(unsigned int v) {id=v; actualizar_representacion();}
	void mut_desp_x(int v) {desp_x=v;}
	void mut_desp_y(int v) {desp_y=v;}

	void mover(int px, int py)
	{
		auto f=[](int val, int c)
		{			
			val+=c;
			if(val < 0) val=0;
			return val;
		};

		x=f(x, px);
		y=f(y, py);

		actualizar_representacion();
	}

	void redimensionar(int pw, int ph)
	{
		w+=pw;
		h+=ph;

		if(w <= 0) w=1;
		if(h <= 0) h=1;

		actualizar_representacion();
	}

	void generar_representacion(const SDL_Renderer * r, const DLibV::Superficie * superf);
	void color_caja(bool seleccion);
	void actualizar_representacion();	

	bool operator<(const Frame&f) const {return id < f.id;}

	bool es_en_posicion(unsigned int px, unsigned int py) const
	{
		return (px >= x && px <= (x + w)) && (py >= y && py <= (y + h));
	}

	friend std::ostream& operator<<(std::ostream& stream, const Frame& f);
	friend std::istream& operator>>(std::istream& stream, Frame& f);
};
//}

struct Frames
{
	std::vector<Frame> frames;

	void sort() {std::sort(std::begin(frames), std::end(frames));}
	unsigned int nuevo_id() const
	{
		if(frames.empty()) return 1;
		else return frames.back().acc_id()+1;
	}

	void push_back(Frame&& f) 
	{
		frames.push_back(std::move(f));
		sort();
	}

	std::vector<Frame>::iterator erase(std::vector<Frame>::iterator it) {return frames.erase(it);}
	void clear() {frames.clear();}
	size_t size() const {return frames.size();}
};

//std::ostream& operator<<(std::ostream& stream, const Aplicacion::Frame& f);

#endif
