Peer = {}
Peer.__index = Peer

function Peer.create(modelName, tag)
	local p = {}
	setmetatable(p, Peer)
	p.id = peerID
	p.fwd = 0
	p.right = 0
	p.model = GO.loadIQM("guy.iqm","player".. peerID)
	p.model:setBoxBody()
	p.model:setActivation(true)
	p.model:setMass(100)
	p.model:setPos(0,10,0)
	p.model:lockAxis(0,0,0)
	peerID = peerID + 1
	return p
end

function Peer.getPeer(list, id)
	for k,v in pairs(list) do
		if v.id  == id then
			return v
		end
	end
	return nil
end
