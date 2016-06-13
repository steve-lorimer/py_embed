class Script(object):
    def __init__(self, ifc):
        print 'created script'
        self.ifc = ifc

    def run(self):
        print 'running'
        self.ifc.execute(5)

    def result(self, i):
        print 'result={}'.format(i)
