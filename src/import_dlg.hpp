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
#ifndef _IMPORT_DLG_H_INCLUDED_
#define _IMPORT_DLG_H_INCLUDED_

// wxwindows
#include "wx/dialog.h"

class ImportDlg:public wxDialog
{
public:
  struct sData
  {
    sData();
    
    wxString Repository;
    wxString Path;
    wxString NewEntry;
    wxString LogMessage;
    bool Recursive;
    bool TreeType, FileType; 
  };
  
  ImportDlg (wxWindow *parent, sData* pData);
  void OnOk (wxCommandEvent & event);
  void OnBrowse (wxCommandEvent & event);

private:  
  void InitializeData ();
  sData * m_data;

  DECLARE_EVENT_TABLE ()
};

#endif
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
