float4x4 = {}


function float4x4:disp(a)
	if a == nil then a = self end
	print(a.m[1],  a.m[2],  a.m[3],  a.m[4])
	print(a.m[5],  a.m[6],  a.m[7],  a.m[8])
	print(a.m[9],  a.m[10], a.m[11], a.m[12])
	print(a.m[13], a.m[14], a.m[15], a.m[16])
end

function float4x4:ident(a)
	if a == nil then a = self end
	a.m[1] = 1; a.m[2]  = 0; a.m[3]  = 0; a.m[4]  = 0
	a.m[5] = 0; a.m[6]  = 1; a.m[7]  = 0; a.m[8]  = 0
	a.m[9] = 0; a.m[10] = 0; a.m[11] = 1; a.m[12] = 0
	a.m[13]= 0; a.m[14] = 0; a.m[15] = 0; a.m[16] = 1
end

function float4x4:translate(x, y, z)
	local m = self.m
	m[13]= x; m[14] = y; m[15] = z;
end

function float4x4:scale(x, y, z)
	local m = self.m
	m[1] = x;
	m[6] = y;
	m[11] = z;
end

function float4x4.new()
	local obj = {
		m = {}
	}
	local tbl = {
		__index = float4x4,
		__add = float4x4__add, 
		__sub = float4x4__sub,
		__mul = float4x4__mul,
		__div = float4x4__div,
	}
	float4x4:ident(obj)
	return setmetatable(obj, tbl)
end



local m0 = float4x4.new()
local m1 = float4x4.new()

m0:disp()
m0:scale(2,3,4)
m0:disp()





