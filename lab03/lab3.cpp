#include <wx/wxprec.h>
#include <wx/colordlg.h>
#include <wx/dcbuffer.h>
#include <wx/filefn.h> 

class MyApp : public wxApp
{
public:
	virtual bool OnInit();
};

class MyFrame : public wxFrame
{
public:
	MyFrame(wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("GFK Lab 03"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize(600, 400), long style = wxDEFAULT_FRAME_STYLE | wxTAB_TRAVERSAL);
	~MyFrame();
private:
	void WxButtonSave_Click(wxCommandEvent& e);
	void WxCheckboxBanana_Click(wxCommandEvent& e);
	void WxScrollbarBanana_Scroll(wxScrollEvent& e);
	void WxButtonStar_Click(wxCommandEvent& e);
	void WxTextctrlText_Updated(wxCommandEvent& e);
	void Form_Paint(wxPaintEvent& e);
	void Form_Update(wxUpdateUIEvent& e);

	void Draw();

	enum {
		ID_WXBUTTON_SAVE = 1001,
		ID_WXCHECKBOX_BANANA = 1002,
		ID_WXSCROLLBAR_BANANA = 1003,
		ID_WXGAUGE_BANANA = 1004,
		ID_WXBUTTON_STAR = 1005,
		ID_WXTEXTCTRL_TEXT = 1006,
		ID_WXCHOISE_CELESTIAL = 1007,
	};

	wxPanel* _panelDrawing;
	wxButton* _buttonSave;
	wxCheckBox* _checkboxBanana;
	wxScrollBar* _scrollbarBanana;
	wxGauge* _gaugeBanana;
	wxButton* _buttonStar;
	wxTextCtrl* _textctrlText;
	wxChoice* _choiceCelestial;

	wxPoint star[5] = {
			wxPoint(-60, -80),
			wxPoint(-35, -35),
			wxPoint(-90, -60),
			wxPoint(-30, -60),
			wxPoint(-80, -35)
	};

	wxImage _imageSave;
	wxBitmap _bitmapBuffer;
	wxImage _imageBanana;
	wxBitmap _bitmapBanana;
	wxString _text = "tekst";
	wxColour _colorStar = wxColour(*wxBLACK);
};

wxIMPLEMENT_APP(MyApp);

bool MyApp::OnInit()
{
	MyFrame* frame = new MyFrame(NULL);
	frame->Show(true);
	return true;
}

MyFrame::MyFrame(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style) : wxFrame(parent, id, title, pos, size, style)
{
	this->SetSizeHints(wxDefaultSize, wxDefaultSize);

	wxBoxSizer* _sizerWindow;
	_sizerWindow = new wxBoxSizer(wxHORIZONTAL);

	wxBoxSizer* _sizerPanel;
	_sizerPanel = new wxBoxSizer(wxVERTICAL);

	_panelDrawing = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);
	_panelDrawing->SetBackgroundColour(wxColour(255, 255, 255));

	_sizerPanel->Add(_panelDrawing, 1, wxEXPAND | wxALL, 5);


	_sizerWindow->Add(_sizerPanel, 1, wxEXPAND, 5);

	wxBoxSizer* _sizerGUI;
	_sizerGUI = new wxBoxSizer(wxVERTICAL);

	_buttonSave = new wxButton(this, ID_WXBUTTON_SAVE, wxT("Zapis do pliku"), wxDefaultPosition, wxDefaultSize, 0);
	_sizerGUI->Add(_buttonSave, 0, wxALL | wxALIGN_CENTER_HORIZONTAL, 5);

	_checkboxBanana = new wxCheckBox(this, ID_WXCHECKBOX_BANANA, wxT("banan"), wxDefaultPosition, wxDefaultSize, 0);
	_sizerGUI->Add(_checkboxBanana, 0, wxALL | wxALIGN_CENTER_HORIZONTAL, 5);

	_scrollbarBanana = new wxScrollBar(this, ID_WXSCROLLBAR_BANANA, wxDefaultPosition, wxDefaultSize, wxSB_HORIZONTAL);
	_scrollbarBanana->SetScrollbar(0, 1, 100, 1);
	_scrollbarBanana->Disable();
	_sizerGUI->Add(_scrollbarBanana, 0, wxALL | wxALIGN_CENTER_HORIZONTAL | wxEXPAND, 5);

	_gaugeBanana = new wxGauge(this, ID_WXGAUGE_BANANA, 100, wxDefaultPosition, wxDefaultSize, wxGA_HORIZONTAL);
	_gaugeBanana->SetValue(0);
	_sizerGUI->Add(_gaugeBanana, 0, wxALL | wxALIGN_CENTER_HORIZONTAL, 5);

	_buttonStar = new wxButton(this, ID_WXBUTTON_STAR, wxT("kolor gwiazdki"), wxDefaultPosition, wxDefaultSize, 0);
	_sizerGUI->Add(_buttonStar, 0, wxALL | wxALIGN_CENTER_HORIZONTAL, 5);

	_textctrlText = new wxTextCtrl(this, ID_WXTEXTCTRL_TEXT, _text, wxDefaultPosition, wxDefaultSize, 0);
	_sizerGUI->Add(_textctrlText, 0, wxALL | wxALIGN_CENTER_HORIZONTAL, 5);

	wxArrayString _choiceCelestialChoices;
	_choiceCelestial = new wxChoice(this, ID_WXCHOISE_CELESTIAL, wxDefaultPosition, wxDefaultSize, _choiceCelestialChoices, 0);
	_choiceCelestial->Append("gwiazdka");
	_choiceCelestial->Append("ksiê¿yc");
	_choiceCelestial->Append("s³onko");
	_choiceCelestial->SetSelection(0);
	_sizerGUI->Add(_choiceCelestial, 0, wxALL | wxALIGN_CENTER_HORIZONTAL, 5);


	_sizerWindow->Add(_sizerGUI, 0, wxEXPAND, 5);


	this->SetSizer(_sizerWindow);
	this->Layout();

	this->Centre(wxBOTH);

	_imageBanana.AddHandler(new wxPNGHandler);
	_imageBanana.LoadFile("C:\\Users\\msatu\\Downloads\\lab03\\banan.png", wxBITMAP_TYPE_PNG);
	
	_bitmapBanana = wxBitmap(_imageBanana);



	Bind(wxEVT_BUTTON, &MyFrame::WxButtonSave_Click, this, ID_WXBUTTON_SAVE);
	Bind(wxEVT_CHECKBOX, &MyFrame::WxCheckboxBanana_Click, this, ID_WXCHECKBOX_BANANA);
	Bind(wxEVT_SCROLL_THUMBTRACK, &MyFrame::WxScrollbarBanana_Scroll, this, ID_WXSCROLLBAR_BANANA);
	Bind(wxEVT_BUTTON, &MyFrame::WxButtonStar_Click, this, ID_WXBUTTON_STAR);
	Bind(wxEVT_TEXT, &MyFrame::WxTextctrlText_Updated, this, ID_WXTEXTCTRL_TEXT);
	Bind(wxEVT_PAINT, &MyFrame::Form_Paint, this);
	Bind(wxEVT_UPDATE_UI, &MyFrame::Form_Update, this);
}

MyFrame::~MyFrame() {
	delete _panelDrawing;
	delete _buttonSave;
	delete _checkboxBanana;
	delete _scrollbarBanana;
	delete _gaugeBanana;
	delete _buttonStar;
	delete _textctrlText;
	delete _choiceCelestial;
}

void MyFrame::WxButtonSave_Click(wxCommandEvent& WXUNUSED(e)) {
	wxFileDialog saveFileDialog(this, "Choose a file", "", "", "All files (*.*)|*.*", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
	if (saveFileDialog.ShowModal() == wxID_CANCEL)
		return;

	_imageSave = _bitmapBuffer.ConvertToImage();
	_imageSave.AddHandler(new wxJPEGHandler);
	_imageSave.AddHandler(new wxPNGHandler);
	_imageSave.SaveFile(saveFileDialog.GetPath());
}

void MyFrame::WxCheckboxBanana_Click(wxCommandEvent& WXUNUSED(e)) {
	_scrollbarBanana->Enable(_checkboxBanana->IsChecked());
}

void MyFrame::WxScrollbarBanana_Scroll(wxScrollEvent& WXUNUSED(e)) {
	_gaugeBanana->SetValue(_scrollbarBanana->GetThumbPosition());
	Draw();
}

void MyFrame::WxButtonStar_Click(wxCommandEvent& WXUNUSED(e)) {
	wxColourDialog chooseColourDialog(this, nullptr);
	if (chooseColourDialog.ShowModal() == wxID_CANCEL)
		return;

	_colorStar = chooseColourDialog.GetColourData().GetColour();
}

void MyFrame::WxTextctrlText_Updated(wxCommandEvent& WXUNUSED(e)) {
	_text = _textctrlText->GetValue();
}

void MyFrame::Form_Paint(wxPaintEvent& WXUNUSED(e)) {
	Draw();
}

void MyFrame::Form_Update(wxUpdateUIEvent& WXUNUSED(e)) {
	Draw();
}

void MyFrame::Draw() {
	std::unique_ptr<wxClientDC> clientDC(new wxClientDC(_panelDrawing));
	_bitmapBuffer = wxBitmap(_panelDrawing->GetSize()); // move to resize
	std::unique_ptr<wxBufferedDC> MyDC(new wxBufferedDC(clientDC.get(), _bitmapBuffer));

	MyDC->SetDeviceOrigin(_panelDrawing->GetSize().x / 2, _panelDrawing->GetSize().y / 2);

	MyDC->SetBackground(*wxWHITE_BRUSH);
	MyDC->Clear();

	double bananaFactor = 0.1 * _gaugeBanana->GetValue() * sin(M_PI * _gaugeBanana->GetValue() / 100);

	MyDC->DrawLine(0, 0, 0, 70); // body
	MyDC->DrawLine(0, 10, -40 - bananaFactor, 30 - 0.4 * _gaugeBanana->GetValue()); //right arm
	MyDC->DrawLine(0, 10, 40, 30); // left arm
	MyDC->DrawLine(0, 70, -30, 100); // right leg
	MyDC->DrawLine(0, 70, 30, 100); // left leg

	if (_checkboxBanana->IsChecked())
		MyDC->DrawBitmap(_bitmapBanana, -40 - bananaFactor - _bitmapBanana.GetWidth() / 2, 30 - 0.4 * _gaugeBanana->GetValue() - _bitmapBanana.GetWidth() / 2, true);

	MyDC->DrawCircle(0, -20, 20);

	MyDC->DrawEllipse(-12, -29, 9, 6);
	if (_checkboxBanana->IsChecked()) {
		MyDC->DrawEllipticArc(-14, -16, 28, 5, 180, 360);
		MyDC->DrawEllipse(3, -29, 9, 6);
	}
	else {
		MyDC->DrawEllipticArc(-14, -16, 28, 5, 0, 180);
		MyDC->DrawEllipse(4, -31, 6, 9);
	}

	wxString text(_text);
	MyDC->DrawText(text, -90, 90);
	wxString textRotated(_text);
	MyDC->SetFont(wxFont(40, wxFONTFAMILY_DECORATIVE, wxFONTSTYLE_ITALIC, wxFONTWEIGHT_BOLD, false, "Brush Script MT"));
	MyDC->DrawRotatedText(_text, 50, 100, 90);

	switch (_choiceCelestial->GetSelection())
	{
	case 0:
		MyDC->SetBrush(wxBrush(_colorStar));
		MyDC->SetPen(wxPen(_colorStar));
		MyDC->DrawPolygon(5, star, 0, 0);
		break;

	case 1:
		MyDC->SetBrush(*wxLIGHT_GREY_BRUSH);
		MyDC->SetPen(*wxLIGHT_GREY_PEN);
		MyDC->DrawCircle(-70, -70, 20);
		MyDC->SetBrush(*wxWHITE_BRUSH);
		MyDC->SetPen(*wxWHITE_PEN);
		MyDC->DrawCircle(-80, -80, 20);
		MyDC->SetBrush(*wxBLACK_BRUSH);
		MyDC->SetPen(*wxBLACK_PEN);
		MyDC->DrawCircle(-60, -70, 2);
		MyDC->DrawLine(-70, -63, -64, -59);
		break;

	case 2:
		MyDC->SetBrush(*wxYELLOW_BRUSH);
		MyDC->SetPen(*wxYELLOW_PEN);
		MyDC->DrawCircle(-70, -70, 20);
		break;

	default:
		break;
	}
}