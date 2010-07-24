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


class Object
    
    def gen_eval(*objs, &block)
      raise "block needed" if !block

      objs = [self] if objs.empty?

      raise "cannot gen_eval on Object" if objs.include?(Object)

      mirror_context = block.__context__.__mirror__
      mirror_context.gen_extend(*objs)
      mirror_context.__set_hidden_self__(self)

      begin
          m = mirror_context.is_a?(Module) ? :module_eval : :instance_eval
          result = mirror_context.send(m, &block)
      ensure
          mirror_context.__remove_hidden_self__
          mirror_context.__unmirror__
      end

      result
    end

    alias_method :gen_eval_with, :gen_eval

    def capture(&block)
        raise "block needed" if !block

        hidden_self = self.__retrieve_hidden_self__

        if hidden_self
            result = hidden_self.instance_eval(&block)
        else
            result = yield
        end

        result
    end
end



  
