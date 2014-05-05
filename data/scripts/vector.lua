Vector = {}
Vector.__index = Vector

function Vector.create(x,y,z)
	local vector = {}
	setmetatable(vector, Vector)
	vector.x = x
	vector.y = y
	vector.z = z
	return vector
end

function Vector.__tostring(vec)
	return vec.x .. " " .. vec.y .. " " .. vec.z
end

function Vector.__concat(lhs,rhs)
	return tostring(lhs)..tostring(rhs)
end

function Vector.__add(lhs,rhs)
	local vector = Vector.create(0,0,0)
	vector.x = lhs.x + rhs.x
	vector.y = lhs.y + rhs.y
	vector.z = lhs.z + rhs.z
	return vector
end

function Vector.__sub(lhs, rhs)
	local vector = Vector.create(0,0,0)
	vector.x = lhs.x - rhs.x
	vector.y = lhs.y - rhs.y
	vector.z = lhs.z - rhs.z
	return vector
end

function Vector.__eq(lhs, rhs)
	if lhs.x == rhs.x and lhs.y == rhs.y and lhs.z == rhs.z then
		return true
	end
	return false
end

function Vector.dot(lhs, rhs)
	local out = lhs.x*rhs.x + lhs.y*rhs.y + lhs.z*rhs.z
	return out
end

function Vector.cross(lhs, rhs)
	local vector = Vector.create(0,0,0)
	vector.x = lhs.y*rhs.z - lhs.z*rhs.y
	vector.y = lhs.z*rhs.x - lhs.x*rhs.z
	vector.z = lhs.x*rhs.y - lhs.y*rhs.x
	return vector
end

function Vector.scalarMul(lhs, rhs)
	local vector = Vector.create(0,0,0)
	vector.x = lhs*rhs.x
	vector.y = lhs*rhs.y
	vector.z = lhs*rhs.z
	return vector
end

function Vector.normalize(vec)
	local vector = Vector.create(0,0,0)
	mag = math.sqrt(vec.x*vec.x + vec.y*vec.y + vec.z*vec.z)
	vector.x = vec.x/mag
	vector.y = vec.y/mag
	vector.z = vec.z/mag
	return vector
end

function Vector:get()
	return self.x, self.y, self.z
end
