/*
 * ====================================================================
 * Copyright (c) 2002, 2003 The RapidSvn Group.  All rights reserved.
 *
 * This software is licensed as described in the file LICENSE.txt,
 * which you should have received as part of this distribution.
 *
 * This software consists of voluntary contributions made by many
 * individuals.  For exact contribution history, see the revision
 * history and logs, available at http://rapidsvn.tigris.org/.
 * ====================================================================
 */

// wxwindows
#include "wx/wx.h"
#include "wx/valgen.h"

// app
#include "import_dlg.hpp"
#include "utils.hpp"

enum
{
  ID_BUTTON_BROWSE = 100,
};

BEGIN_EVENT_TABLE (ImportDlg, wxDialog)
  EVT_BUTTON (wxID_OK, ImportDlg::OnOk)
  EVT_BUTTON (ID_BUTTON_BROWSE, ImportDlg::OnBrowse)
END_EVENT_TABLE ()

ImportDlg::sData::sData()
{
  // Default values go here:
  TreeType = true;
  FileType = !TreeType;
  Recursive = true;
}

ImportDlg::ImportDlg (wxWindow * parent, sData* pData)
  : wxDialog (parent, -1, "Import an unversioned file or tree", 
              wxDefaultPosition, 
              wxDefaultSize, wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER),
    m_data(pData)
{
  InitializeData ();
  CentreOnParent();
}

void
ImportDlg::OnOk (wxCommandEvent &event)
{
  wxString val;

    // Transfer data from controls into m_data:
  TransferDataFromWindow();

  TrimString (m_data->Repository);
  TrimString (m_data->Path);
  TrimString (m_data->NewEntry);
  TrimString (m_data->LogMessage);
  
  if (m_data->Repository.IsEmpty ())
  {
    wxMessageBox (_("Repository URL is required for import!"),
                  _("Error"), wxOK | wxCENTRE | wxICON_ERROR);

    // Do not allow the user to continue if the path is empty 
    // and the import is addressing a file.
    return;
  }

  if (m_data->FileType)
  {

    if (m_data->Path.IsEmpty ())
    {
      wxMessageBox (_("File path required when importing a file!"),
                    _("Error"), wxOK | wxCENTRE | wxICON_ERROR);

      // Do not allow the user to continue if the reposURL is empty.
      return;
    }

    if (m_data->NewEntry.IsEmpty ())
    {
      wxMessageBox (_("New entry name required when importing a file!"),
                    _("Error"), wxOK | wxCENTRE | wxICON_ERROR);

      // Do not allow the user to continue if the new_entry is empty 
      // and the import is addressing a file.
      return;
    }
  }

  wxDialog::OnOK(event);
}

void
ImportDlg::OnBrowse (wxCommandEvent & WXUNUSED (event))
{
  // Transfer data from controls into m_data:
  TransferDataFromWindow();
  
  if (m_data->TreeType)
  {
    wxDirDialog dialog (this,
                        _("Select a directory to import"),
                        m_data->Path);

    if (dialog.ShowModal () == wxID_OK)
      m_data->Path = dialog.GetPath ();
  }
  else                          // it's a file 
  {
    wxFileDialog dialog (this,
                         _("Select a file to import"),
                         m_data->Path);

    if (dialog.ShowModal () == wxID_OK)
      m_data->Path = dialog.GetPath ();
  }
  // Transfer data from m_data back into controls:
  TransferDataToWindow();
}

void
ImportDlg::InitializeData ()
{
  wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);

  wxFlexGridSizer* Grid = new wxFlexGridSizer(2, 3, 5, 0);
  Grid->AddGrowableCol(1);  // The second column can be expanded.

  // Row 0:  
  Grid->Add(new wxStaticText(this, -1, _("Repository URL")), 0, 
    wxLEFT | wxALIGN_CENTER_VERTICAL, 5);
  wxTextCtrl *Repository = new wxTextCtrl(this, -1, "",
    wxDefaultPosition, wxSize(300, -1), 0,
    wxTextValidator(wxFILTER_NONE, &m_data->Repository));
  Grid->Add(Repository, 1, wxLEFT | wxEXPAND | wxALIGN_CENTER_VERTICAL, 5);
  Grid->Add(new wxStaticText(this, -1, ""), 0, 
    0, 5);

  // Row 1:  
  Grid->Add(new wxStaticText(this, -1, _("Path")), 0, 
    wxLEFT | wxALIGN_CENTER_VERTICAL, 5);
  wxTextCtrl *Path = new wxTextCtrl(this, -1, "",
    wxDefaultPosition, wxSize(300, -1), 0,
    wxTextValidator(wxFILTER_NONE, &m_data->Path));
  Grid->Add(Path, 1, wxLEFT | wxEXPAND, 5);
  wxButton* BrowseButton = new wxButton(this, ID_BUTTON_BROWSE, _("..."), 
    wxPoint(-1,-1), wxSize(20, -1));
  Grid->Add(BrowseButton, 0, wxLEFT | wxALIGN_CENTER_VERTICAL, 5);
  
  // Row 2:  
  Grid->Add(new wxStaticText(this, -1, _("New entry")), 0, 
    wxLEFT | wxALIGN_CENTER_VERTICAL, 5);
  wxTextCtrl *NewEntry = new wxTextCtrl(this, -1, "",
    wxDefaultPosition, wxSize(300, -1), 0,
    wxTextValidator(wxFILTER_NONE, &m_data->NewEntry));
  Grid->Add(NewEntry, 1, wxLEFT | wxEXPAND | wxALIGN_CENTER_VERTICAL, 5);
  Grid->Add(new wxStaticText(this, -1, ""), 0, 
    0, 5);

  mainSizer->Add(Grid, 0, wxALL | wxEXPAND, 5);

  
  // The message field:
  wxStaticBoxSizer *messageSizer = new wxStaticBoxSizer(
    new wxStaticBox(this, -1, _("Enter log message")), wxHORIZONTAL);
    
  wxTextCtrl* Log = new wxTextCtrl(this, -1, "", wxDefaultPosition, 
    wxSize(-1, 50), wxTE_MULTILINE,
    wxTextValidator(wxFILTER_NONE, &m_data->LogMessage));
    
  messageSizer->Add(Log, 1, wxALL | wxEXPAND, 5);
  
  mainSizer->Add(messageSizer, 1, wxALL | wxEXPAND, 5);

  // Sundry items row:
  wxBoxSizer *SundrySizer = new wxBoxSizer(wxHORIZONTAL);
  
  wxCheckBox* Recursive = new wxCheckBox (this, -1, _("Recursive"),
    wxDefaultPosition, wxDefaultSize, 0, wxGenericValidator(&m_data->Recursive));
  SundrySizer->Add(Recursive, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);
  
  SundrySizer->Add(new wxStaticText(this, -1, _("Path type:")), 0, 
    wxLEFT | wxALIGN_CENTER_VERTICAL, 30);

  SundrySizer->Add(
    new wxRadioButton(this, -1, _("Tree"), wxDefaultPosition, wxDefaultSize, 0,
      wxGenericValidator(&m_data->TreeType)),
    0, wxALL | wxALIGN_CENTER_VERTICAL, 5);
  SundrySizer->Add(
    new wxRadioButton(this, -1, _("File"), wxDefaultPosition, wxDefaultSize, 0,
      wxGenericValidator(&m_data->FileType)),
    0, wxALL | wxALIGN_CENTER_VERTICAL, 5);

  mainSizer->Add(SundrySizer, 0, wxALL | wxCENTER, 5);

  // Button row
  wxBoxSizer *ButtonSizer = new wxBoxSizer(wxHORIZONTAL);
  ButtonSizer->Add(new wxButton(this, wxID_OK, _("OK" )), 0, 
    wxALL, 10);
  ButtonSizer->Add(new wxButton(this, wxID_CANCEL, _("Cancel")), 0, 
    wxALL, 10);

  mainSizer->Add(ButtonSizer, 0, wxALL | wxCENTER, 5);

  SetAutoLayout(true);
  SetSizer(mainSizer);

  mainSizer->SetSizeHints(this);
  mainSizer->Fit(this);
}
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
