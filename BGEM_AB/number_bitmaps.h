const unsigned char PROGMEM number00_bitmap[] =
{
  //    
  // ###
  // # #
  // # #
  // # #
  // # #
  // ###
  //  # 
  0x7E, 0xC2, 0x7E, 
};

const unsigned char PROGMEM number01_bitmap[] =
{
  //    
  //  # 
  // ## 
  //  # 
  //  # 
  //  # 
  // ###
  //  # 
  0x44, 0xFE, 0x40, 
};

const unsigned char PROGMEM number02_bitmap[] =
{
  //    
  // ###
  //   #
  // ###
  // #  
  // #  
  // ###
  //  # 
  0x7A, 0xCA, 0x4E, 
};

const unsigned char PROGMEM number03_bitmap[] =
{
  //    
  // ###
  //   #
  //  ##
  //   #
  //   #
  // ###
  //  # 
  0x42, 0xCA, 0x7E, 
};

const unsigned char PROGMEM number04_bitmap[] =
{
  //    
  // #  
  // # #
  // # #
  // ###
  //   #
  //   #
  //  # 
  0x1E, 0x90, 0x7C, 
};

const unsigned char PROGMEM number05_bitmap[] =
{
  //    
  // ###
  // #  
  // ###
  //   #
  //   #
  // ###
  //  # 
  0x4E, 0xCA, 0x7A, 
};

const unsigned char PROGMEM number06_bitmap[] =
{
  //    
  // ###
  // #  
  // ###
  // # #
  // # #
  // ###
  //  # 
  0x7E, 0xCA, 0x7A, 
};


const unsigned char PROGMEM number07_bitmap[] =
{
  //    
  // ###
  //   #
  //   #
  //  # 
  //  # 
  //  # 
  //  # 
  0x02, 0xF2, 0x0E, 
};

const unsigned char PROGMEM number08_bitmap[] =
{
  //    
  // ###
  // # #
  // ###
  // # #
  // # #
  // ###
  //  # 
  0x7E, 0xCA, 0x7E, 
};

const unsigned char PROGMEM number09_bitmap[] =
{
  //    
  // ###
  // # #
  // ###
  //   #
  //   #
  // ###
  //  # 
  0x4E, 0xCA, 0x7E, 
};


const unsigned char PROGMEM numberMask_bitmap[] =
{
  // ###
  // ###
  // ###
  // ###
  // ###
  // ###
  // ###
  // ###
  0xFF, 0xFF, 0xFF, 
};

const unsigned char PROGMEM numberEmpty_bitmap[] =
{
  //
  //
  //
  //
  //
  //
  //
  //
  0x00, 0x00, 0x00, 
};

const unsigned char *numberBlink_bitmaps[] = {
  numberMask_bitmap, numberEmpty_bitmap, numberEmpty_bitmap, numberEmpty_bitmap,
  numberMask_bitmap, numberEmpty_bitmap, numberEmpty_bitmap, numberEmpty_bitmap,
  numberMask_bitmap, numberEmpty_bitmap, numberMask_bitmap, numberEmpty_bitmap,
  numberMask_bitmap, numberEmpty_bitmap, numberMask_bitmap, numberEmpty_bitmap,
};

const unsigned char *number_bitmaps[] = {
  number00_bitmap, number01_bitmap, number02_bitmap,
  number03_bitmap, number04_bitmap, number05_bitmap,
  number06_bitmap, number07_bitmap, number08_bitmap,
  number09_bitmap
};

const unsigned char PROGMEM numberBig00_bitmap[] =
{
  //   #### 
  //  ######
  //  ##  ##
  // ##   ##
  // ##   ##
  // ##  ## 
  // ###### 
  //  ####  
  0x78, 0xFE, 0xC7, 0xC3, 0xE3, 0x7F, 0x1E, 
};

const unsigned char PROGMEM numberBig01_bitmap[] =
{
  //    ##  
  //  ####  
  //  # ##  
  //    #   
  //    #   
  //   ##   
  //   ##   
  //   ##   
  0x00, 0x06, 0xE2, 0xFF, 0x07, 0x00, 0x00, 
};

const unsigned char PROGMEM numberBig02_bitmap[] =
{
  //    ### 
  //   #####
  //  ##  ##
  //      # 
  //    ##  
  //   ##   
  //  ##### 
  // ###### 
  0x80, 0xC4, 0xE6, 0xF3, 0xD3, 0xCF, 0x06, 
};

const unsigned char PROGMEM numberBig03_bitmap[] =
{
  //    ### 
  //   #####
  //  ##  ##
  //    ### 
  //    ####
  //  ##  ##
  //  ######
  //   #### 
  0x00, 0x64, 0xE6, 0xDB, 0xDB, 0xFF, 0x76, 
};

const unsigned char PROGMEM numberBig04_bitmap[] =
{
  //     ## 
  //    ### 
  //   #### 
  //  ## ## 
  // ##  ## 
  // #######
  //  ##### 
  //    ##  
  0x30, 0x78, 0x6C, 0xE6, 0xFF, 0x7F, 0x20, 
};

const unsigned char PROGMEM numberBig05_bitmap[] =
{
  //    ####
  //   #####
  //   ##   
  //   #### 
  //    ####
  // ##   ##
  // #######
  //  ##### 
  0x60, 0xE0, 0xCE, 0xDF, 0xDB, 0xFB, 0x73, 
};

const unsigned char PROGMEM numberBig06_bitmap[] =
{
  //     ###
  //    ####
  //   ###  
  //  ##### 
  // #######
  // ##   ##
  // #######
  //  ####  
  0x70, 0xF8, 0xDC, 0xDE, 0xDF, 0x7B, 0x73, 
};

const unsigned char PROGMEM numberBig07_bitmap[] =
{
  //  ######
  //  ######
  //     ## 
  //     ## 
  //    ##  
  //   ##   
  //  ###   
  // ###    
  0x80, 0xC3, 0xE3, 0x73, 0x1F, 0x0F, 0x03, 
};

const unsigned char PROGMEM numberBig08_bitmap[] =
{
  //   #### 
  //  ######
  //  ##  ##
  //   #####
  //  ##### 
  // ##  ## 
  // ###### 
  //  ####  
  0x60, 0xF6, 0xDF, 0xDB, 0xFB, 0x7F, 0x0E, 
};

const unsigned char PROGMEM numberBig09_bitmap[] =
{
  //   #### 
  //   #####
  //  ##  ##
  //  ##  ##
  //   #### 
  //    ##  
  // #####  
  //  ###   
  0x40, 0xCC, 0xDF, 0xF3, 0x73, 0x1F, 0x0E, 
};

const unsigned char *numberBig_bitmaps[] = {
  numberBig00_bitmap, numberBig01_bitmap, numberBig02_bitmap,
  numberBig03_bitmap, numberBig04_bitmap, numberBig05_bitmap,
  numberBig06_bitmap, numberBig07_bitmap, numberBig08_bitmap,
  numberBig09_bitmap
};
