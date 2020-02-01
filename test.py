import requests

def setup():
	pass

def teardown():
	pass

g_url = 'http://192.168.122.1:8000'


def test_http_put_0_byte():
	data = ''
	ret = requests.put(g_url, data)
	assert(ret.status_code == 200)

def test_http_put_1_byte():
	data = '1'
	ret = requests.put(g_url, data)
	assert(ret.status_code == 200)

def test_http_put_3_bytes():
	data = '123'
	ret = requests.put(g_url, data)
	assert(ret.status_code == 200)

def test_http_put_1023_bytes():
	data = '1'*1023
	ret = requests.put(g_url, data)
	assert(ret.status_code == 200)

def test_http_put_815_bytes():
	data = '1'*815
	ret = requests.put(g_url, data)
	assert(ret.status_code == 200)

def test_http_put_1K():
	data = '1'*1024
	ret = requests.put(g_url, data)
	assert(ret.status_code == 200)

def test_http_put_1K_and_1():
	data = '1'*(1024+1)
	ret = requests.put(g_url, data)
	assert(ret.status_code == 200)

def test_http_put_0_byte_and_get():
	data = ''
	ret = requests.put(g_url, data)
	ret = requests.get(g_url)
	assert data == ret.text

def test_http_put_1_byte_and_get():
	data = '1'
	ret = requests.put(g_url, data)
	ret = requests.get(g_url)
	assert data == ret.text

def test_http_put_3_bytes_and_get():
	data = '123'
	ret = requests.put(g_url, data)
	ret = requests.get(g_url)
	assert data == ret.text

def test_http_put_1023_bytes_and_get():
	data = '1'*1023
	ret = requests.put(g_url, data)
	ret = requests.get(g_url)
	assert data == ret.text

def test_http_put_815_bytes_and_get():
	data = '1'*815
	ret = requests.put(g_url, data)
	ret = requests.get(g_url)
	assert data == ret.text

def test_http_put_1K_and_get():
	data = '1'*1024
	ret = requests.put(g_url, data)
	ret = requests.get(g_url)
	assert data == ret.text

def test_http_put_1K_and_1_and_get():
	data = '1'*1024
	ret = requests.put(g_url, data+'x')
	ret = requests.get(g_url)
	assert data == ret.text

