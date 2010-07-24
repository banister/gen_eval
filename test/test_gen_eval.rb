require 'test/unit'

require File.dirname(__FILE__) + "/../lib/gen_eval"

class Gen_EvalTest < Test::Unit::TestCase
    def setup
        @p = Proc.new do
            assert_equal :bink, bink
            assert_equal :local_method, local_method
        end

        @p2 = Proc.new do
            assert_equal :bink, bink
            assert_equal :bunk, bunk
            assert_equal :local_method, local_method
        end

        @m = Module.new do
            def bink
                :bink
            end
        end

        @o = Object.new
        class << @o
            def bink
                :bink
            end
        end

        @o2 = Object.new
        class << @o2
            def bunk
                :bunk
            end
        end

        @c = Class.new
        @c.class_eval do
            def bink
                :bink
            end
        end
    end

    #a method in the local binding
    #used for testing
    def local_method
        :local_method
    end

    def test_mixing_in_self_when_object
        o = Object.new
        class << o
            def bink
                :bink
            end
        end

        o.gen_eval(&@p)
    end

    def test_mixing_in_self_when_class
        c = Class.new
        c.class_eval do
            def bink
                :bink
            end
        end

        c.gen_eval(&@p)
    end

    def test_mixing_in_module
           Module.new.gen_eval_with(@m, &@p)
    end

    def test_mixing_in_object
           Module.new.gen_eval_with(@o, &@p)
    end

    def test_mixing_in_class
           Module.new.gen_eval_with(@c, &@p)
    end

    def test_mixing_in_multi_objects
           Module.new.gen_eval_with(@o, @o2, &@p2)
    end
    
    def test_capture_for_gen_eval
        o = Object.new
        class << o
            def set_x(v)
                @x = v
            end

            def get_x
                @x
            end
        end

       o.gen_eval { set_x(777) }

        assert_equal nil, o.get_x
        
        def o.set_x(v)
            capture {
                @x = v
            }
        end

        o.gen_eval { set_x(777) }

        assert_equal 777, o.get_x
    end
    
    def test_capture_without_gen_eval
        o = Object.new
        class << o
            def set_x(v)
                capture { 
                    @x = v
                }
            end

            def get_x
                @x
            end
        end

        o.set_x(777) 

        assert_equal 777, o.get_x
    end

    def test_modification_of_ivars

        @ivar = 20

        o = Object.new
        class << o
            def set_x(v)
                capture { 
                    @x = v
                }
            end

            def get_x
                @x
            end     
        end

        o.gen_eval {
            @ivar = 50
        }

        assert_equal 50, @ivar
    end
        
end
