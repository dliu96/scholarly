
import hashlib
import getpass

username = raw_input('Username: ')
password = getpass.getpass()

pwhash = hashlib.md5(password).hexdigest()

pathname = "auth/%s" % (username)
f = open(pathname, "w+")
f.write(pwhash)
f.close()

pathname = "account/%s" % (username)
f = open(pathname, "w+")
f.write("10000")
f.close()

