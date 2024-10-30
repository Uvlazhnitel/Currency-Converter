#include <wx/grid.h> // Подключаем wxGrid
#include <wx/wx.h>
#include <wx/mstream.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <map>

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
    wxFlexGridSizer* gridSizer; // Изменено на wxGridSizer
    wxBoxSizer* boxSizer;
    wxImage image;
    int row;

    std::map<std::string,float> currencyPrice = {
        {"EUR",0.92},
        {"RUB",97.02},
        {"BTC",0.000014},
        {"YUAN",7.12}

    };

    std::multimap<std::string,std::string> currencyBox = {
        {"USD","EUR"},
        {"USD","RUB"},
        {"USD","BTC"},
        {"USD","YUAN"},
    };

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
    MyFrame* frame = new MyFrame("Converter");
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
    wxColour textColor(34, 47, 62); 

    textCtrl->SetForegroundColour(textColor);
    textCtrl1->SetForegroundColour(textColor);


    staticTextCtrl = new wxStaticText(panel, wxID_ANY, "USD:", wxPoint(50, 20), wxDefaultSize, wxALIGN_LEFT);

    staticTextCtrl1 = new wxStaticText(panel, wxID_ANY, "", wxPoint(410, 20), wxDefaultSize, wxALIGN_LEFT);

    staticTextCtrl->SetForegroundColour(textColor);
    staticTextCtrl1->SetForegroundColour(textColor);

    textCtrl->Bind(wxEVT_TEXT, &MyFrame::OnTextChanged, this);

    comboBoxItems.Add("EUR");
    // comboBoxItems.Add("EUR-USD");
    // comboBoxItems.Add("EUR-RUB");
    // comboBoxItems.Add("EUR-BTC");

    for (const auto& box : currencyBox) {
        comboBoxItems.Add(wxString::Format("%s-%s", box.first, box.second));
    };

    comboBox = new wxComboBox(panel, wxID_ANY, "Select Item", wxPoint(50, 50), wxSize(150, 30), comboBoxItems, wxCB_READONLY);

    comboBox->SetSelection(0);
    comboBox->Bind(wxEVT_COMBOBOX, &MyFrame::OnComboBoxSelection, this);

    comboBox->SetForegroundColour(textColor); 

    boxSizer = new wxBoxSizer(wxHORIZONTAL);
    boxSizer->Add(textCtrl, 0, wxTOP | wxLEFT, 40);
    boxSizer->Add(comboBox, 0, wxALL, 30);
    boxSizer->Add(textCtrl1, 0, wxTOP , 40);

    row = currencyPrice.size();

    gridSizer = new wxFlexGridSizer(row,1, 5, 2); 
    for (const auto& pair : currencyPrice) {
        wxStaticText* label = new wxStaticText(panel, wxID_ANY, wxString::Format("%s: %g", pair.first, pair.second));
        label->SetForegroundColour(textColor);
        gridSizer->Add(label, 0, wxALIGN_CENTER);
    }

    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
    mainSizer->Add(boxSizer, 0, wxALL | wxEXPAND , 10);
    mainSizer->Add(gridSizer, 0, wxLEFT, 50);
    
    panel->SetSizer(mainSizer);

    Refresh();
}

void MyFrame::UpdateConversion(wxCommandEvent& event) {
    int selection = comboBox->GetSelection();

    if (selection == 1) {
        staticTextCtrl1->SetLabel("EUR:");
        textCtrl1->SetValue(wxString::Format("%.2f", intValue * currencyPrice["EUR"]));
    } else if (selection == 2) {
        staticTextCtrl1->SetLabel("RUB:");
        textCtrl1->SetValue(wxString::Format("%.2f", intValue * currencyPrice["RUB"]));
    } else if (selection == 3) {
        staticTextCtrl1->SetLabel("BTC:");
        textCtrl1->SetValue(wxString::Format("%.6f", intValue * currencyPrice["BTC"]));
    } else if (selection == 4) {
        staticTextCtrl1->SetLabel("YUAN:");
        textCtrl1->SetValue(wxString::Format("%.2f", intValue * currencyPrice["YUAN"]));
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
