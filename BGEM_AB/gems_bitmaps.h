const unsigned char PROGMEM gem00inverted_bitmap[] =
{
  // ########
  // ########
  // ########
  // ########
  // ########
  // ########
  // ########
  // ########
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
};


const unsigned char PROGMEM gem01inverted_bitmap[] =
{
  // ########
  // ##    ##
  // #  ##  #
  // # #  # #
  // # #  # #
  // #  ##  #
  // ##    ##
  // ########
  0xFF, 0xC3, 0x99, 0xA5, 0xA5, 0x99, 0xC3, 0xFF, 
};


const unsigned char PROGMEM gem02inverted_bitmap[] =
{
  // ########
  // ## ## ##
  // # #  # #
  // # #### #
  // # #### #
  // ## ## ##
  // ###  ###
  // ########
  0xFF, 0xE3, 0xDD, 0xBB, 0xBB, 0xDD, 0xE3, 0xFF, 
};


const unsigned char PROGMEM gem03inverted_bitmap[] =
{
  // ########
  // #  ##  #
  // #      #
  // ## ## ##
  // ## ## ##
  // #      #
  // #  ##  #
  // ########
  0xFF, 0x99, 0x81, 0xDB, 0xDB, 0x81, 0x99, 0xFF, 
};


const unsigned char PROGMEM gem04inverted_bitmap[] =
{
  // ########
  // ##    ##
  // # ###  #
  // # ##   #
  // # #    #
  // #      #
  // ##    ##
  // ########
  0xFF, 0xC3, 0x9D, 0x8D, 0x85, 0x81, 0xC3, 0xFF, 
};


const unsigned char PROGMEM gem05inverted_bitmap[] =
{
  // ########
  // #     ##
  // # # ## #
  // #    # #
  // # #    #
  // # ## # #
  // ##     #
  // ########
  0xFF, 0xC1, 0xB5, 0xA1, 0x85, 0xAD, 0x83, 0xFF, 
};


const unsigned char PROGMEM gem06inverted_bitmap[] =
{
  // ########
  // ###  ###
  // ##    ##
  // #  #   #
  // #      #
  // ##    ##
  // ###  ###
  // ########
  0xFF, 0xE7, 0xC3, 0x89, 0x81, 0xC3, 0xE7, 0xFF, 
};


const unsigned char PROGMEM gem07inverted_bitmap[] =
{
  // ########
  // ##    ##
  // # #### #
  // #   #  #
  // #  #   #
  // # #### #
  // ##    ##
  // ########
  0xFF, 0xC3, 0xA5, 0xB5, 0xAD, 0xA5, 0xC3, 0xFF, 
};


const unsigned char PROGMEM gem08inverted_bitmap[] =
{
  // ########
  // ##   ###
  // #  ## ##
  // # #  # #
  // # ## # #
  // #  ##  #
  // #     ##
  // ########
  0xFF, 0x83, 0x99, 0xB5, 0xA5, 0x9B, 0xC7, 0xFF, 
};


const unsigned char *gemsinverted_bitmaps[] =
{
  gem00inverted_bitmap,
  gem01inverted_bitmap, gem02inverted_bitmap, gem03inverted_bitmap, gem04inverted_bitmap,
  gem05inverted_bitmap, gem06inverted_bitmap, gem07inverted_bitmap, gem08inverted_bitmap
};

const unsigned char PROGMEM selector00_bitmap[] =
{
  // # # # # 
  //        #
  // #       
  //        #
  // #       
  //        #
  // #       
  //  # # # #
  0x55, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0xAA, 
};


const unsigned char PROGMEM selector01_bitmap[] =
{
  //  # # # #
  // #       
  //        #
  // #       
  //        #
  // #       
  //        #
  // # # # # 
  0xAA, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x55, 
};


const unsigned char *selector_bitmaps[] =
{
  selector00_bitmap, selector01_bitmap,
};


