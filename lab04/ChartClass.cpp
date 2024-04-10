#include <wx/dc.h>
#include <memory>

#include "ChartClass.h"
#include "vecmat.h"

ChartClass::ChartClass(std::shared_ptr<ConfigClass> c)
{
	cfg = std::move(c);
	x_step = 200;
}

void ChartClass::Set_Range()
{
	double xmin = 9999.9, xmax = -9999.9, ymin = 9999.9, ymax = -9999.9;
	double x, y, step;
	int i;

	xmin = cfg->Get_x_start();
	xmax = cfg->Get_x_stop();

	step = (cfg->Get_x_stop() - cfg->Get_x_start()) / (double)x_step;
	x = cfg->Get_x_start();

	for (i = 0; i <= x_step; i++)
	{
		y = GetFunctionValue(x);
		if (y > ymax) ymax = y;
		if (y < ymin) ymin = y;
		x = x + step;
	}

	y_min = ymin;
	y_max = ymax;
	x_min = xmin;
	x_max = xmax;
}


double ChartClass::GetFunctionValue(double x)
{
	if (cfg->Get_F_type() == 1) return x * x;
	if (cfg->Get_F_type() == 2) return 0.5 * exp(4 * x - 3 * x * x);
	return x + sin(x * 4.0);
}

void ChartClass::Draw(wxDC* dc, int w, int h)
{
	dc->SetBackground(wxBrush(RGB(255, 255, 255)));
	dc->Clear();
	dc->SetPen(wxPen(RGB(255, 0, 0)));
	dc->DrawRectangle(10, 10, w - 20, h - 20);
	dc->SetClippingRegion(wxRect(10, 10, w - 20, h - 20));

	this->w = w;
	this->h = h;
	this->dc = dc;

	const double xStart = cfg->Get_x_start(), yMin = Get_Y_min(), xStop = cfg->Get_x_stop(), yMax = Get_Y_max();
	double dx = (xStop - xStart) / x_step;

	// Macierz transformacji
	Matrix t;
	if (cfg->RotateScreenCenter())
		t = generateRotationMatrix() * generateTranslationMatrix() * generateScreenOriginMatrix();
	else
		t = generateTranslationMatrix() * generateRotationMatrix() * generateScreenOriginMatrix();

	// Osie wykresu
	dc->SetPen(wxPen(RGB(0, 0, 255)));
	line2d(t, xStart, 0, xStop, 0);
	line2d(t, 0, yMin, 0, yMax);

	// Strza³ki
	line2d(t, xStop, 0, xStop - 0.05, 0.05);
	line2d(t, xStop, 0, xStop - 0.05, -0.05);
	line2d(t, 0, yMax, 0.025, yMax - 0.1);
	line2d(t, 0, yMax, -0.025, yMax - 0.1);

	// Znaczniki x
	for (double x = xStart; x < xStop; x += 0.5) {
		if (x == 0)
			continue;
		line2d(t, x, -0.05, x, 0.05);
		drawText(t, x, x - 0.05, -0.05);
	}

	// Znaczniki y
	for (double y = yMin + 0.7; y < yMax; y += 1) {
		line2d(t, -0.025, y, 0.025, y);
		drawText(t, y, 0.025, y);
	}

	// Wykres
	dc->SetPen(wxPen(RGB(0, 255, 0)));
	for (double x = xStart; x <= xStop; x += dx) {
		line2d(t, x, GetFunctionValue(x), x + dx, GetFunctionValue(x + dx));
	}
}

void ChartClass::line2d(Matrix t, double x1, double y1, double x2, double y2)
{
	Vector u, v;

	u.Set(x1, y1);
	v.Set(x2, y2);
	u = t * u;
	v = t * v;

	dc->DrawLine(u.GetX(), u.GetY(), v.GetX(), v.GetY());
}

double ChartClass::Get_Y_min()
{
	Set_Range();
	return y_min;
}

double ChartClass::Get_Y_max()
{
	Set_Range();
	return y_max;
}

void ChartClass::drawText(Matrix t, double value, double x, double y) {
	Vector text;

	text.Set(x, y);
	text = t * text;

	dc->DrawRotatedText(wxString::Format(wxT("%.2g"), value), text.GetX(), text.GetY(), cfg->Get_Alpha());
}

Matrix ChartClass::generateScreenOriginMatrix() {
	Matrix m;

	double sx = w / ((cfg->Get_x1()) - (cfg->Get_x0()));
	double sy = h / ((cfg->Get_y1()) - (cfg->Get_y0())) * -1.0;

	m.data[0][0] = sx;
	m.data[0][2] = -(cfg->Get_x0()) * sx - 10;

	m.data[1][1] = sy;
	m.data[1][2] = -(cfg->Get_y1()) * sy + 10;

	return m;
}

Matrix ChartClass::generateTranslationMatrix() {
	Matrix m;

	m.data[0][0] = 1;
	m.data[0][2] = cfg->Get_dX();

	m.data[1][1] = 1;
	m.data[1][2] = cfg->Get_dY();

	return m;
}

Matrix ChartClass::generateRotationMatrix() {
	Matrix m;

	double a = -cfg->Get_Alpha() * M_PI / 180.0;

	m.data[0][0] = cos(a);
	m.data[0][1] = -sin(a);
	m.data[0][2] = w / 2 * (1 - cos(a)) + h / 2 * sin(a);

	m.data[1][0] = sin(a);
	m.data[1][1] = cos(a);
	m.data[1][2] = -w / 2 * sin(a) + h / 2 * (1 - cos(a));

	return m;
}