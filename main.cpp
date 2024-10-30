#include <wx/grid.h> // Подключаем wxGrid
#include <wx/wx.h>
#include <wx/mstream.h>
#include <wx/bitmap.h>
#include <wx/image.h>

class MyFrame : public wxFrame
{
public:
    MyFrame(const wxString& title);

private:
    wxTextCtrl* textCtrl;
    wxTextCtrl* textCtrl1;
    wxArrayString comboBoxItems;
    wxComboBox* comboBox;
    wxStaticText* staticTextCtrl;
    wxStaticText* staticTextCtrl1;
    wxGridSizer* gridSizer; // Изменено на wxGridSizer
    wxBoxSizer* boxSizer;
    wxImage image;

    double intValue;

    void OnComboBoxSelection(wxCommandEvent& event);
    void OnTextChanged(wxCommandEvent& event);
    void UpdateConversion(wxCommandEvent& event);

};

class MyApp : public wxApp
{
public:
    virtual bool OnInit();
};

wxIMPLEMENT_APP(MyApp);

bool MyApp::OnInit()
{
    MyFrame* frame = new MyFrame("Basic wxWidgets App");
    frame->Show(true);
    return true;
}

MyFrame::MyFrame(const wxString& title)
    : wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(600, 300))
{
    SetBackgroundColour(wxColour(173, 216, 230));
    wxPanel* panel = new wxPanel(this, wxID_ANY);

    textCtrl = new wxTextCtrl(panel, wxID_ANY, "", wxPoint(10, 10), wxSize(150, 30));
    textCtrl->SetBackgroundColour(wxColour(144, 238, 144));

    textCtrl1 = new wxTextCtrl(panel, wxID_ANY, "", wxPoint(10, 50), wxSize(150, 30), wxTE_READONLY);
    textCtrl1->SetBackgroundColour(wxColour(144, 238, 144));
    wxColour textColor(144, 238, 144); 

    textCtrl->SetForegroundColour(textColor);
    textCtrl1->SetForegroundColour(textColor);


    staticTextCtrl = new wxStaticText(panel, wxID_ANY, "EUR:", wxPoint(50, 20), wxDefaultSize, wxALIGN_LEFT);

    staticTextCtrl1 = new wxStaticText(panel, wxID_ANY, "", wxPoint(410, 20), wxDefaultSize, wxALIGN_LEFT);

    staticTextCtrl->SetForegroundColour(textColor);
    staticTextCtrl1->SetForegroundColour(textColor);

    textCtrl->Bind(wxEVT_TEXT, &MyFrame::OnTextChanged, this);

    comboBoxItems.Add("EUR");
    comboBoxItems.Add("EUR-USD");
    comboBoxItems.Add("EUR-RUB");
    comboBoxItems.Add("EUR-BTC");

    comboBox = new wxComboBox(panel, wxID_ANY, "Select Item", wxPoint(50, 50), wxSize(150, 30), comboBoxItems, wxCB_READONLY);

    comboBox->SetSelection(0);
    comboBox->Bind(wxEVT_COMBOBOX, &MyFrame::OnComboBoxSelection, this);

    boxSizer = new wxBoxSizer(wxHORIZONTAL);
    boxSizer->Add(textCtrl, 0, wxTOP | wxLEFT, 40);
    boxSizer->Add(comboBox, 0, wxALL, 30);
    boxSizer->Add(textCtrl1, 0, wxTOP , 40);

    gridSizer = new wxGridSizer(3, 3, 2, 2); 
    for (int i = 1; i <= 9; ++i) {
        wxStaticText* label = new wxStaticText(panel, wxID_ANY, wxString::Format("Text %d", i)); 
        gridSizer->Add(label, 0, wxALIGN_CENTER);
    }

    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
    mainSizer->Add(boxSizer, 0, wxALL | wxEXPAND, 10);
    mainSizer->Add(gridSizer, 0, wxALL | wxEXPAND, 10);
    
    panel->SetSizer(mainSizer);


    
    Refresh();
}

void MyFrame::UpdateConversion(wxCommandEvent& event) {
    int selection = comboBox->GetSelection();

    if (selection == 1) {
        staticTextCtrl1->SetLabel("USD:");
        textCtrl1->SetValue(wxString::Format("%.2f", intValue * 1.08));
    } else if (selection == 2) {
        staticTextCtrl1->SetLabel("RUB:");
        textCtrl1->SetValue(wxString::Format("%.2f", intValue * 104.55));
    } else if (selection == 3) {
        staticTextCtrl1->SetLabel("BTC:");
        textCtrl1->SetValue(wxString::Format("%.6f", intValue / 61956));
    } else {
        textCtrl1->Clear();
        staticTextCtrl1->SetLabel("");
    }
}

void MyFrame::OnComboBoxSelection(wxCommandEvent& event) {
    UpdateConversion(event);
}

void MyFrame::OnTextChanged(wxCommandEvent& event) {
    wxString text = textCtrl->GetValue();

    if (text.ToDouble(&intValue)) {
        textCtrl1->SetValue(wxString::Format("%.2f", intValue));
        UpdateConversion(event);
    } else if (text.IsEmpty()) {
        textCtrl1->Clear();
    }
}
