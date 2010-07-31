# (C) John Mair 2009, under the MIT licence

require 'rbconfig'
require 'object2module'

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

class Proc  #:nodoc:
    def __context__ 
        eval('self', self.binding)
    end
end


class Object
    
  #  call-seq:
  #     obj.gen_eval {| | block }                       => obj
  #  
  #  Works similarly to instance_eval except instance variables are
  #  not looked up in _obj_. All instance methods of +Klass+ are
  #  available but mutators must be wrapped in a +capture+ block.
  #     
  #     class Klass
  #       attr_reader :age
  #  
  #       def hello(name)
  #         "hello #{name}"
  #       end
  #
  #       def set_age(age)
  #         capture { 
  #           @age = age
  #         }
  #       end
  #     end
  #     k = Klass.new
  #     @name = "John"
  #     k.gen_eval { hello(@name) }   #=> "hello John"
  #
  #     k.gen_eval { set_age(21) }
  #     k.age #=> 21
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
      
    #  call-seq:
    #     capture {| | block }                       => result
    #  
    #  Used in conjunction with +gen_eval+. Causes code in block
    #  to be +instance_eval+'d with respect to the receiver of
    #  the +gen_eval+.
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



  
