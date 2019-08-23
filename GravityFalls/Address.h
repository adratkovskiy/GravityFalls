#pragma once
class Address
{
public:
	Address();
	Address(unsigned char a, unsigned char b, unsigned char c, unsigned char d, unsigned short port);
	Address(unsigned int address, unsigned short port);
	unsigned int GetAddress() const;
	unsigned char GetA() const;
	unsigned char GetB() const;
	unsigned char GetC() const;
	unsigned char GetD() const;
	unsigned short GetPort() const;
	bool operator == (const Address& other) const
	{
		return this->address == other.address && this->port == other.port;
	}

	bool operator != (const Address& other) const
	{
		return !(*this == other);
	}


private:
	unsigned int address;
	unsigned short port;
};

