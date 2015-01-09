      program test
      implicit none
      
      call HFillSetup(0,1,10)      
      call HFill(0,0.3d0,120d0,0.1d0,.false.)
      call HFill(0,0.3d0,120d0,0.1d0,.true.)
      call HFill(0,0.6d0,480d0,0.2d0,.false.)
      call HFillExport()
      
      end program
