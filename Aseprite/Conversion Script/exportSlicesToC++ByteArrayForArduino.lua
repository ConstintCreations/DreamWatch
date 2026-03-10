local sprite = app.activeSprite;
if not sprite then
    app.alert("No Active Sprite")
    return
end

local home = os.getenv("HOME") or os.getenv("USERPROFILE")
local filepath = home .. "/Downloads/bitmaps.h"

local file = io.open(filepath, "w")
if not file then
  app.alert("Failed to open file: "..filepath)
  return
end

file:write("#pragma once\n")

local function sanitize(name)
  name = name:gsub("%s+", "_")
  if string.find(name, "^%d") then
    name = "num"..name
  end
  return name
end

local function byteToBinary(b)
  local s = ""
  for i = 7, 0, -1 do
    if (b & (1 << i)) ~= 0 then
      s = s .. "1"
    else
      s = s .. "0"
    end
  end
  return "0b" .. s
end

for _,slice in ipairs(sprite.slices) do
  local name = sanitize(slice.name)

  local bounds = slice.bounds
  local w = bounds.width
  local h = bounds.height

  file:write("#define W_"..name:upper().." "..w.."\n")
  file:write("#define H_"..name:upper().." "..h.."\n")

  file:write("static const unsigned char PROGMEM "..name.."[] = {\n")

  local image = sprite.cels[1].image

  for y=0,h-1 do
    local byte = 0
    local bit = 7
    local lineBytes = {}

    for x=0,w-1 do
      local px = image:getPixel(bounds.x+x, bounds.y+y)

      local a = app.pixelColor.rgbaA(px)
      local r = app.pixelColor.rgbaR(px)
      local g = app.pixelColor.rgbaG(px)
      local b = app.pixelColor.rgbaB(px)

      if a > 0 and not (r == 0 and g == 0 and b == 0) then
        byte = byte | (1 << bit)
      end

      bit = bit - 1

      if bit < 0 then
        table.insert(lineBytes, byteToBinary(byte))
        byte = 0
        bit = 7
      end
    end

    if bit ~= 7 then
      table.insert(lineBytes, byteToBinary(byte))
    end

    file:write("  "..table.concat(lineBytes,", ")..",\n")
  end

  file:write("};\n\n")
end

file:close()

app.alert("Exported to "..filepath)