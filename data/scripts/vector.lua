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
	mag = vec:mag()
	vector.x = vec.x/mag
	vector.y = vec.y/mag
	vector.z = vec.z/mag
	return vector
end

function Vector:mag()
	return math.sqrt(self.x*self.x + self.y*self.y + self.z*self.z)  
end

function Vector:get()
	return self.x, self.y, self.z
end

function Vector:rotate(angle, axis)
	return Vector.scalarMul(math.cos(math.rad(angle)),self) 
		+ Vector.scalarMul(math.sin(math.rad(angle)), (Vector.cross(axis,self))) 
		+ Vector.scalarMul(Vector.dot(axis,self)*(1-math.cos(math.rad(angle))),axis)
end

function Vector.distance(vec1,vec2)
	subtracted = vec1-vec2
	return subtracted:mag()
end

function axisToEuler(vec, angle)
	angle = math.rad(angle)
	local out = Vector.create(0,0,0)
	out.y = math.atan2(vec.y * math.sin(angle)-vec.x*vec.z*(1-math.cos(angle)), 1-(vec.y*vec.y + vec.z*vec.z)*(1-math.cos(angle)))
	out.x = math.asin(vec.x*vec.y*(1-math.cos(angle))+vec.z*math.sin(angle))
	out.y = math.atan2(vec.x*math.sin(angle)-vec.y*vec.z*(1-math.cos(angle)), 1 - (vec.x*vec.x + vec.z*vec.z)*(1-math.cos(angle)))
	return out
end
