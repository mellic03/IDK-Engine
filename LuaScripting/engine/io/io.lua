local this = { };

-- SDL2 keybindings from SDL_scancode.h --

this.scancodes = {

  A = 4,
  B = 5,
  C = 6,
  D = 7,
  E = 8,
  F = 9,
  G = 10,
  H = 11,
  I = 12,
  J = 13,
  K = 14,
  L = 15,
  M = 16,
  N = 17,
  O = 18,
  P = 19,
  Q = 20,
  R = 21,
  S = 22,
  T = 23,
  U = 24,
  V = 25,
  W = 26,
  X = 27,
  Y = 28,
  Z = 29,

  KB_1 = 30,
  KB_2 = 31,
  KB_3 = 32,
  KB_4 = 33,
  KB_5 = 34,
  KB_6 = 35,
  KB_7 = 36,
  KB_8 = 37,
  KB_9 = 38,
  KB_0 = 39,

  KB_RETURN = 40,
  KB_ESCAPE = 41,
  KB_BACKSPACE = 42,
  KB_TAB = 43,
  KB_SPACE = 44,

  KB_LCTRL = 224,
  KB_LSHIFT = 225,
  KB_LALT = 226,
  KB_LGUI = 227, -- windows, command (apple), meta --
  KB_RCTRL = 228,
  KB_RSHIFT = 229,
  KB_RALT = 230, -- alt gr, option --
  KB_RGUI = 231, -- windows, command (apple), meta --


  __index = function (scancodeTable, key)
    return scancodeTable[this[key] + 1];
  end ;
};



this.initKeyPressed = function(keyPressedTable)

  setmetatable(keyPressedTable, {
    __index = function(table, key)
      return table[this.scancodes[key] + 1];
    end;
  });

end;




return this;

