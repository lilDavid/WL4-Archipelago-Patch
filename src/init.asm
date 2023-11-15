.gba


; HardwareInitialization()
hook 0x8000728, 0x8000738, InitializeVariables


.autoregion :: .importobj "obj/init.o" :: .endautoregion
