#pragma once

/*
	Types of layouts to be scared of:

    Swing: https://docs.oracle.com/javase/tutorial/uiswing/layout/visual.html
        BorderLayout
        BoxLayout
        CardLayout
        FlowLayout
        GridBagLayout
        GridLayout
        GroupLayout
        SpringLayout

    WinForms:
        FlowLayoutPanel
        GroupBox
        Panel
        SplitContainer
        TabControl
        TableLayoutPanel

    QtWidgets: https://doc.qt.io/qt-6/layout.html

    GGLib:
        Flow
        Grid
        Stack
        None/freeform/absolute



    1.
    Panels are containers
    Containers are collections of controls, and other containers
    e.g., window (container) 
            |
          Flow Panel (container)
            |
          Button, Button, Button

    2.
    Panels manage the position/sizing of their child controls
    Requires negotiation/consultation with those child controls since e.g., a control might say it is anchored to the left, or has a max width
    To do this: multiple walks through the tree

    
    Properties of controls that affect layout:
        Anchoring
        Desired/max/min size  

    Properties of containers that affect layout:
        Layout type (flow, grid, none, etc.)

*/

#include "ui/UIControl.h"

class Panel
{
private:
	int x, y, w, h;
    

public:
    void add(UIControl uiControl)
    {
        
    }




};
