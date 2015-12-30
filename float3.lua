float3 = {}

-- operator
function float3__add(a, b)
	if type(b) == "number" then b = float3.new(b,b,b) end
	return float3.new(a.x + b.x, a.y + b.y, a.z + b.z)
end

function float3__sub(a, b)
	if type(b) == "number" then b = float3.new(b,b,b) end
	return float3.new(a.x - b.x, a.y - b.y, a.z - b.z)
end

function float3__mul(a, b)
	if type(b) == "number" then b = float3.new(b,b,b) end
	return float3.new(a.x * b.x, a.y * b.y, a.z * b.z)
end

function float3__div(a, b)
	if type(b) == "number" then b = float3.new(b,b,b) end
	return float3.new(a.x / b.x, a.y / b.y, a.z / b.z)
end

-- other
function float3:dot(a, b)
	return a.x * b.x + a.y * b.y + a.z * b.z
end

function float3:len(a)
	if a == nil then a = self end
	local d = self:dot(a, a)
	return math.sqrt(d)
end

function float3:normalize(a)
	if a == nil then
		a= self;
	end
	local il = 1.0 / a:len()
	a.x = a.x * il
	a.y = a.y * il
	a.z = a.z * il
end

function float3:cross(a, b)
	return float3.new(
	(a.y * b.z) - (a.z * b.y),
	(a.z * b.x) - (a.x * b.z),
	(a.x * b.y) - (a.y * b.x))
end

function float3:disp(a)
	if a == nil then a = self end
	print(self.x, self.y, self.z)
end

function float3.new(a, b, c)
	local obj = { x = a, y = b, z = c }
	local tbl = {
		__index = float3,
		__add = float3__add, 
		__sub = float3__sub,
		__mul = float3__mul,
		__div = float3__div,
	}
	return setmetatable(obj, tbl)
end

