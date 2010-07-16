# (C) John Mair 2009, under the MIT licence

require 'rbconfig'

direc = File.dirname(__FILE__)
dlext = Config::CONFIG['DLEXT']

begin
    if RUBY_VERSION && RUBY_VERSION =~ /1.9/
        require "#{direc}/1.9/gen_eval.#{dlext}"
    else
        require "#{direc}/1.8/gen_eval.#{dlext}"
    end
rescue LoadError => e
    require "#{direc}/gen_eval.#{dlext}"
end
require "#{direc}/gen_eval/version"

class Proc  
    def __context__
        eval('self', self.binding)
    end
end


