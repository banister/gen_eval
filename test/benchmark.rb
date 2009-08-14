require 'benchmark'
require '../lib/gen_eval'

module TexPlayMock
    def pixel
    end
    
    def line
    end
    
    def circle
    end
end

module GosuMock
    class ImageMock
        include TexPlayMock
    end
end


n = 10000

Benchmark.bm do |x|
    x.report("gen_eval") {
        n.times {
            o = GosuMock::ImageMock.new     
            o.gen_eval { line; pixel; circle; }
        }
    }
    
    x.report("instance_eval") {
        n.times {
            o = GosuMock::ImageMock.new     
            o.instance_eval { line; pixel; circle; }
        }
    }

    x.report("ordinary method invocation") {
        n.times {
            o = GosuMock::ImageMock.new
            o.line
            o.pixel
            o.circle
        }
    }
end
    
    
    
        
