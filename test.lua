
require("float3")


local a0 = float3.new(1,1,0)
local a1 = float3.new(0,1,1)
local a2 = float3.new(1,-1,0)

CreateWindow(12345, 56789)

float3:normalize(a0)
float3:normalize(a1)
float3:normalize(a2)
a0:disp()
a1:disp()
a2:disp()

print("sasaki"); print("sasakimasaki");

local c  = float3:cross(a0, a1)
local angle = float3:dot(a0, a1)
c:disp()
a0:disp()

a0 = a0 + 10
a0:disp()

a0 = a0 - 7
a0:disp()


a0 = a0 * 5
a0:disp()

a0 = a0 / 2
a0:disp()

print(a0:len(a0))
local a2 = float3:cross(a0, a1)
a2:disp()

obj = {}

function obj.test(a)
	print("sakana") 
end

obj.test(123)


