#!/usr/bin/env python
# -*- coding: utf-8 -*-
# generated by wxGlade 0.6.3 on Tue Jul 14 14:59:35 2009

import wx

# begin wxGlade: extracode
# end wxGlade

MEMORY = None

class MainFrame(wx.Frame):
    def __init__(self, *args, **kwds):
        # begin wxGlade: MainFrame.__init__
        kwds["style"] = wx.DEFAULT_FRAME_STYLE
        wx.Frame.__init__(self, *args, **kwds)
        self.notebook_central = wx.Notebook(self, -1, style=wx.NB_BOTTOM)
        self.notebook_topright = wx.Notebook(self, -1, style=wx.NB_RIGHT)
        self.notebook_bottomright = wx.Notebook(self, -1, style=wx.NB_RIGHT)
        self.scrub_slider = wx.Slider(self, -1, 0, 0, 10)

        self.__set_properties()
        self.__do_layout()

        self.Bind(wx.EVT_COMMAND_SCROLL, self.scrub_slider_handler, self.scrub_slider)
        # end wxGlade
        
        self.slider_changed_func = None

    def __set_properties(self):
        # begin wxGlade: MainFrame.__set_properties
        self.SetTitle("main_frame")
        self.SetSize((1000, 600))
        self.scrub_slider.SetMinSize((300, 17))
        # end wxGlade

    def __do_layout(self):
        # begin wxGlade: MainFrame.__do_layout
        sizer_3 = wx.BoxSizer(wx.HORIZONTAL)
        sizer_4 = wx.BoxSizer(wx.VERTICAL)
        sizer_6 = wx.BoxSizer(wx.HORIZONTAL)
        sizer_5 = wx.BoxSizer(wx.HORIZONTAL)
        sizer_2 = wx.BoxSizer(wx.HORIZONTAL)
        sizer_1 = wx.BoxSizer(wx.HORIZONTAL)
        sizer_1.Add(self.notebook_central, 1, wx.EXPAND, 0)
        sizer_3.Add(sizer_1, 2, wx.EXPAND, 0)
        sizer_2.Add(self.notebook_topright, 1, wx.EXPAND, 0)
        sizer_4.Add(sizer_2, 3, wx.EXPAND, 0)
        sizer_5.Add(self.notebook_bottomright, 1, wx.EXPAND, 0)
        sizer_4.Add(sizer_5, 3, wx.EXPAND, 0)
        sizer_6.Add(self.scrub_slider, 0, wx.ALIGN_CENTER_HORIZONTAL|wx.ALIGN_CENTER_VERTICAL, 0)
        sizer_4.Add(sizer_6, 1, wx.ALIGN_CENTER_VERTICAL|wx.ADJUST_MINSIZE, 2)
        sizer_3.Add(sizer_4, 1, wx.EXPAND, 0)
        self.SetSizer(sizer_3)
        self.Layout()
        self.SetSize((1000, 600))
        # end wxGlade

    def scrub_slider_handler(self, event): # wxGlade: MainFrame.<event_handler>
        self.slider_changed_func(self.scrub_slider.GetValue())
        #event.Skip()

# end of class MainFrame


class SimpleView(wx.App):
    def OnInit(self):
        wx.InitAllImageHandlers()
        main_frame = MainFrame(None, -1, "")
        self.SetTopWindow(main_frame)
        main_frame.Show()
        return 1

# end of class SimpleView

if __name__ == "__main__":
    simple_view = SimpleView(0)
    simple_view.MainLoop()