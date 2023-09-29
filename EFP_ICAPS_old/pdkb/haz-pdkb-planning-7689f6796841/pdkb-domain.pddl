(define (domain grapevine)

    (:requirements :strips :conditional-effects)

    (:predicates
        (not_at_a_l1)
        (not_at_a_l2)
        (not_at_b_l1)
        (not_at_b_l2)
        (not_at_c_l1)
        (not_at_c_l2)
        (not_at_d_l1)
        (not_at_d_l2)
        (not_connected_l1_l1)
        (not_connected_l1_l2)
        (not_connected_l2_l1)
        (not_connected_l2_l2)
        (not_secret_a)
        (not_secret_b)
        (not_secret_c)
        (not_secret_d)
        (Ba_not_secret_a)
        (Ba_not_secret_b)
        (Ba_not_secret_c)
        (Ba_not_secret_d)
        (Ba_secret_a)
        (Ba_secret_b)
        (Ba_secret_c)
        (Ba_secret_d)
        (Bb_not_secret_a)
        (Bb_not_secret_b)
        (Bb_not_secret_c)
        (Bb_not_secret_d)
        (Bb_secret_a)
        (Bb_secret_b)
        (Bb_secret_c)
        (Bb_secret_d)
        (Bc_not_secret_a)
        (Bc_not_secret_b)
        (Bc_not_secret_c)
        (Bc_not_secret_d)
        (Bc_secret_a)
        (Bc_secret_b)
        (Bc_secret_c)
        (Bc_secret_d)
        (Bd_not_secret_a)
        (Bd_not_secret_b)
        (Bd_not_secret_c)
        (Bd_not_secret_d)
        (Bd_secret_a)
        (Bd_secret_b)
        (Bd_secret_c)
        (Bd_secret_d)
        (Pa_not_secret_a)
        (Pa_not_secret_b)
        (Pa_not_secret_c)
        (Pa_not_secret_d)
        (Pa_secret_a)
        (Pa_secret_b)
        (Pa_secret_c)
        (Pa_secret_d)
        (Pb_not_secret_a)
        (Pb_not_secret_b)
        (Pb_not_secret_c)
        (Pb_not_secret_d)
        (Pb_secret_a)
        (Pb_secret_b)
        (Pb_secret_c)
        (Pb_secret_d)
        (Pc_not_secret_a)
        (Pc_not_secret_b)
        (Pc_not_secret_c)
        (Pc_not_secret_d)
        (Pc_secret_a)
        (Pc_secret_b)
        (Pc_secret_c)
        (Pc_secret_d)
        (Pd_not_secret_a)
        (Pd_not_secret_b)
        (Pd_not_secret_c)
        (Pd_not_secret_d)
        (Pd_secret_a)
        (Pd_secret_b)
        (Pd_secret_c)
        (Pd_secret_d)
        (at_a_l1)
        (at_a_l2)
        (at_b_l1)
        (at_b_l2)
        (at_c_l1)
        (at_c_l2)
        (at_d_l1)
        (at_d_l2)
        (connected_l1_l1)
        (connected_l1_l2)
        (connected_l2_l1)
        (connected_l2_l2)
        (secret_a)
        (secret_b)
        (secret_c)
        (secret_d)
    )

    (:action move_a_l1_l1
        :precondition (and (at_a_l1)
                           (connected_l1_l1))
        :effect (and
                    ; #28974: origin
                    (not_at_a_l1)

                    ; #35234: origin
                    (at_a_l1)

                    ; #28974: <==negation-removal== 35234 (pos)
                    (not (not_at_a_l1))

                    ; #35234: <==negation-removal== 28974 (pos)
                    (not (at_a_l1))))

    (:action move_a_l1_l2
        :precondition (and (at_a_l1)
                           (connected_l1_l2))
        :effect (and
                    ; #28974: origin
                    (not_at_a_l1)

                    ; #35234: origin
                    (at_a_l2)

                    ; #28974: <==negation-removal== 35234 (pos)
                    (not (not_at_a_l2))

                    ; #35234: <==negation-removal== 28974 (pos)
                    (not (at_a_l1))))

    (:action move_a_l2_l1
        :precondition (and (at_a_l2)
                           (connected_l2_l1))
        :effect (and
                    ; #28974: origin
                    (not_at_a_l2)

                    ; #35234: origin
                    (at_a_l1)

                    ; #28974: <==negation-removal== 35234 (pos)
                    (not (not_at_a_l1))

                    ; #35234: <==negation-removal== 28974 (pos)
                    (not (at_a_l2))))

    (:action move_a_l2_l2
        :precondition (and (at_a_l2)
                           (connected_l2_l2))
        :effect (and
                    ; #28974: origin
                    (not_at_a_l2)

                    ; #35234: origin
                    (at_a_l2)

                    ; #28974: <==negation-removal== 35234 (pos)
                    (not (not_at_a_l2))

                    ; #35234: <==negation-removal== 28974 (pos)
                    (not (at_a_l2))))

    (:action move_b_l1_l1
        :precondition (and (at_b_l1)
                           (connected_l1_l1))
        :effect (and
                    ; #37570: origin
                    (not_at_b_l1)

                    ; #95745: origin
                    (at_b_l1)

                    ; #37570: <==negation-removal== 95745 (pos)
                    (not (not_at_b_l1))

                    ; #95745: <==negation-removal== 37570 (pos)
                    (not (at_b_l1))))

    (:action move_b_l1_l2
        :precondition (and (at_b_l1)
                           (connected_l1_l2))
        :effect (and
                    ; #37570: origin
                    (not_at_b_l1)

                    ; #95745: origin
                    (at_b_l2)

                    ; #37570: <==negation-removal== 95745 (pos)
                    (not (not_at_b_l2))

                    ; #95745: <==negation-removal== 37570 (pos)
                    (not (at_b_l1))))

    (:action move_b_l2_l1
        :precondition (and (at_b_l2)
                           (connected_l2_l1))
        :effect (and
                    ; #37570: origin
                    (not_at_b_l2)

                    ; #95745: origin
                    (at_b_l1)

                    ; #37570: <==negation-removal== 95745 (pos)
                    (not (not_at_b_l1))

                    ; #95745: <==negation-removal== 37570 (pos)
                    (not (at_b_l2))))

    (:action move_b_l2_l2
        :precondition (and (at_b_l2)
                           (connected_l2_l2))
        :effect (and
                    ; #37570: origin
                    (not_at_b_l2)

                    ; #95745: origin
                    (at_b_l2)

                    ; #37570: <==negation-removal== 95745 (pos)
                    (not (not_at_b_l2))

                    ; #95745: <==negation-removal== 37570 (pos)
                    (not (at_b_l2))))

    (:action move_c_l1_l1
        :precondition (and (at_c_l1)
                           (connected_l1_l1))
        :effect (and
                    ; #67431: origin
                    (not_at_c_l1)

                    ; #71975: origin
                    (at_c_l1)

                    ; #67431: <==negation-removal== 71975 (pos)
                    (not (not_at_c_l1))

                    ; #71975: <==negation-removal== 67431 (pos)
                    (not (at_c_l1))))

    (:action move_c_l1_l2
        :precondition (and (connected_l1_l2)
                           (at_c_l1))
        :effect (and
                    ; #67431: origin
                    (not_at_c_l1)

                    ; #71975: origin
                    (at_c_l2)

                    ; #67431: <==negation-removal== 71975 (pos)
                    (not (not_at_c_l2))

                    ; #71975: <==negation-removal== 67431 (pos)
                    (not (at_c_l1))))

    (:action move_c_l2_l1
        :precondition (and (at_c_l2)
                           (connected_l2_l1))
        :effect (and
                    ; #67431: origin
                    (not_at_c_l2)

                    ; #71975: origin
                    (at_c_l1)

                    ; #67431: <==negation-removal== 71975 (pos)
                    (not (not_at_c_l1))

                    ; #71975: <==negation-removal== 67431 (pos)
                    (not (at_c_l2))))

    (:action move_c_l2_l2
        :precondition (and (at_c_l2)
                           (connected_l2_l2))
        :effect (and
                    ; #67431: origin
                    (not_at_c_l2)

                    ; #71975: origin
                    (at_c_l2)

                    ; #67431: <==negation-removal== 71975 (pos)
                    (not (not_at_c_l2))

                    ; #71975: <==negation-removal== 67431 (pos)
                    (not (at_c_l2))))

    (:action move_d_l1_l1
        :precondition (and (connected_l1_l1)
                           (at_d_l1))
        :effect (and
                    ; #61247: origin
                    (at_d_l1)

                    ; #78542: origin
                    (not_at_d_l1)

                    ; #61247: <==negation-removal== 78542 (pos)
                    (not (at_d_l1))

                    ; #78542: <==negation-removal== 61247 (pos)
                    (not (not_at_d_l1))))

    (:action move_d_l1_l2
        :precondition (and (connected_l1_l2)
                           (at_d_l1))
        :effect (and
                    ; #61247: origin
                    (at_d_l2)

                    ; #78542: origin
                    (not_at_d_l1)

                    ; #61247: <==negation-removal== 78542 (pos)
                    (not (at_d_l1))

                    ; #78542: <==negation-removal== 61247 (pos)
                    (not (not_at_d_l2))))

    (:action move_d_l2_l1
        :precondition (and (at_d_l2)
                           (connected_l2_l1))
        :effect (and
                    ; #61247: origin
                    (at_d_l1)

                    ; #78542: origin
                    (not_at_d_l2)

                    ; #61247: <==negation-removal== 78542 (pos)
                    (not (at_d_l2))

                    ; #78542: <==negation-removal== 61247 (pos)
                    (not (not_at_d_l1))))

    (:action move_d_l2_l2
        :precondition (and (at_d_l2)
                           (connected_l2_l2))
        :effect (and
                    ; #61247: origin
                    (at_d_l2)

                    ; #78542: origin
                    (not_at_d_l2)

                    ; #61247: <==negation-removal== 78542 (pos)
                    (not (at_d_l2))

                    ; #78542: <==negation-removal== 61247 (pos)
                    (not (not_at_d_l2))))

    (:action share_a_a_l1
        :precondition (and (at_a_l1)
                           (Ba_secret_a))
        :effect (and
                    ; #36751: origin
                    (when (and (at_c_l1))
                          (Bc_secret_a))

                    ; #40173: origin
                    (when (and (at_b_l1))
                          (Bb_secret_a))

                    ; #49982: <==closure== 40173 (pos)
                    (when (and (at_b_l1))
                          (Pb_secret_a))

                    ; #65285: <==closure== 36751 (pos)
                    (when (and (at_c_l1))
                          (Pc_secret_a))

                    ; #74565: origin
                    (when (and (at_a_l1))
                          (Ba_secret_a))

                    ; #76029: origin
                    (when (and (at_d_l1))
                          (Bd_secret_a))

                    ; #77434: <==closure== 74565 (pos)
                    (when (and (at_a_l1))
                          (Pa_secret_a))

                    ; #85879: <==closure== 76029 (pos)
                    (when (and (at_d_l1))
                          (Pd_secret_a))

                    ; #24085: <==negation-removal== 65285 (pos)
                    (when (and (at_c_l1))
                          (not (Bc_not_secret_a)))

                    ; #25681: <==negation-removal== 77434 (pos)
                    (when (and (at_a_l1))
                          (not (Ba_not_secret_a)))

                    ; #35373: <==uncertain_firing== 76029 (pos)
                    (when (and (not (not_at_d_l1)))
                          (not (Pd_not_secret_a)))

                    ; #44889: <==uncertain_firing== 49982 (pos)
                    (when (and (not (not_at_b_l1)))
                          (not (Bb_not_secret_a)))

                    ; #45173: <==uncertain_firing== 74565 (pos)
                    (when (and (not (not_at_a_l1)))
                          (not (Pa_not_secret_a)))

                    ; #47318: <==negation-removal== 74565 (pos)
                    (when (and (at_a_l1))
                          (not (Pa_not_secret_a)))

                    ; #47557: <==uncertain_firing== 85879 (pos)
                    (when (and (not (not_at_d_l1)))
                          (not (Bd_not_secret_a)))

                    ; #47734: <==uncertain_firing== 36751 (pos)
                    (when (and (not (not_at_c_l1)))
                          (not (Pc_not_secret_a)))

                    ; #52158: <==uncertain_firing== 77434 (pos)
                    (when (and (not (not_at_a_l1)))
                          (not (Ba_not_secret_a)))

                    ; #56215: <==negation-removal== 76029 (pos)
                    (when (and (at_d_l1))
                          (not (Pd_not_secret_a)))

                    ; #57268: <==uncertain_firing== 65285 (pos)
                    (when (and (not (not_at_c_l1)))
                          (not (Bc_not_secret_a)))

                    ; #59894: <==negation-removal== 36751 (pos)
                    (when (and (at_c_l1))
                          (not (Pc_not_secret_a)))

                    ; #70106: <==negation-removal== 85879 (pos)
                    (when (and (at_d_l1))
                          (not (Bd_not_secret_a)))

                    ; #73836: <==uncertain_firing== 40173 (pos)
                    (when (and (not (not_at_b_l1)))
                          (not (Pb_not_secret_a)))

                    ; #77799: <==negation-removal== 40173 (pos)
                    (when (and (at_b_l1))
                          (not (Pb_not_secret_a)))

                    ; #86338: <==negation-removal== 49982 (pos)
                    (when (and (at_b_l1))
                          (not (Bb_not_secret_a)))))

    (:action share_a_a_l2
        :precondition (and (Ba_secret_a)
                           (at_a_l2))
        :effect (and
                    ; #11249: <==closure== 35073 (pos)
                    (when (and (at_a_l2))
                          (Pa_secret_a))

                    ; #19915: origin
                    (when (and (at_b_l2))
                          (Bb_secret_a))

                    ; #35073: origin
                    (when (and (at_a_l2))
                          (Ba_secret_a))

                    ; #44610: <==closure== 72416 (pos)
                    (when (and (at_c_l2))
                          (Pc_secret_a))

                    ; #53454: <==closure== 69245 (pos)
                    (when (and (at_d_l2))
                          (Pd_secret_a))

                    ; #69245: origin
                    (when (and (at_d_l2))
                          (Bd_secret_a))

                    ; #72416: origin
                    (when (and (at_c_l2))
                          (Bc_secret_a))

                    ; #79557: <==closure== 19915 (pos)
                    (when (and (at_b_l2))
                          (Pb_secret_a))

                    ; #14221: <==negation-removal== 11249 (pos)
                    (when (and (at_a_l2))
                          (not (Ba_not_secret_a)))

                    ; #14339: <==negation-removal== 19915 (pos)
                    (when (and (at_b_l2))
                          (not (Pb_not_secret_a)))

                    ; #22461: <==uncertain_firing== 69245 (pos)
                    (when (and (not (not_at_d_l2)))
                          (not (Pd_not_secret_a)))

                    ; #23757: <==negation-removal== 53454 (pos)
                    (when (and (at_d_l2))
                          (not (Bd_not_secret_a)))

                    ; #28915: <==negation-removal== 69245 (pos)
                    (when (and (at_d_l2))
                          (not (Pd_not_secret_a)))

                    ; #31652: <==uncertain_firing== 44610 (pos)
                    (when (and (not (not_at_c_l2)))
                          (not (Bc_not_secret_a)))

                    ; #36397: <==uncertain_firing== 53454 (pos)
                    (when (and (not (not_at_d_l2)))
                          (not (Bd_not_secret_a)))

                    ; #38489: <==negation-removal== 72416 (pos)
                    (when (and (at_c_l2))
                          (not (Pc_not_secret_a)))

                    ; #40408: <==uncertain_firing== 11249 (pos)
                    (when (and (not (not_at_a_l2)))
                          (not (Ba_not_secret_a)))

                    ; #44794: <==uncertain_firing== 19915 (pos)
                    (when (and (not (not_at_b_l2)))
                          (not (Pb_not_secret_a)))

                    ; #55088: <==negation-removal== 35073 (pos)
                    (when (and (at_a_l2))
                          (not (Pa_not_secret_a)))

                    ; #55680: <==uncertain_firing== 72416 (pos)
                    (when (and (not (not_at_c_l2)))
                          (not (Pc_not_secret_a)))

                    ; #61057: <==uncertain_firing== 35073 (pos)
                    (when (and (not (not_at_a_l2)))
                          (not (Pa_not_secret_a)))

                    ; #67298: <==negation-removal== 44610 (pos)
                    (when (and (at_c_l2))
                          (not (Bc_not_secret_a)))

                    ; #76347: <==negation-removal== 79557 (pos)
                    (when (and (at_b_l2))
                          (not (Bb_not_secret_a)))

                    ; #87686: <==uncertain_firing== 79557 (pos)
                    (when (and (not (not_at_b_l2)))
                          (not (Bb_not_secret_a)))))

    (:action share_a_b_l1
        :precondition (and (at_a_l1)
                           (Ba_secret_b))
        :effect (and
                    ; #36751: origin
                    (when (and (at_c_l1))
                          (Bc_secret_b))

                    ; #40173: origin
                    (when (and (at_b_l1))
                          (Bb_secret_b))

                    ; #49982: <==closure== 40173 (pos)
                    (when (and (at_b_l1))
                          (Pb_secret_b))

                    ; #65285: <==closure== 36751 (pos)
                    (when (and (at_c_l1))
                          (Pc_secret_b))

                    ; #74565: origin
                    (when (and (at_a_l1))
                          (Ba_secret_b))

                    ; #76029: origin
                    (when (and (at_d_l1))
                          (Bd_secret_b))

                    ; #77434: <==closure== 74565 (pos)
                    (when (and (at_a_l1))
                          (Pa_secret_b))

                    ; #85879: <==closure== 76029 (pos)
                    (when (and (at_d_l1))
                          (Pd_secret_b))

                    ; #24085: <==negation-removal== 65285 (pos)
                    (when (and (at_c_l1))
                          (not (Bc_not_secret_b)))

                    ; #25681: <==negation-removal== 77434 (pos)
                    (when (and (at_a_l1))
                          (not (Ba_not_secret_b)))

                    ; #35373: <==uncertain_firing== 76029 (pos)
                    (when (and (not (not_at_d_l1)))
                          (not (Pd_not_secret_b)))

                    ; #44889: <==uncertain_firing== 49982 (pos)
                    (when (and (not (not_at_b_l1)))
                          (not (Bb_not_secret_b)))

                    ; #45173: <==uncertain_firing== 74565 (pos)
                    (when (and (not (not_at_a_l1)))
                          (not (Pa_not_secret_b)))

                    ; #47318: <==negation-removal== 74565 (pos)
                    (when (and (at_a_l1))
                          (not (Pa_not_secret_b)))

                    ; #47557: <==uncertain_firing== 85879 (pos)
                    (when (and (not (not_at_d_l1)))
                          (not (Bd_not_secret_b)))

                    ; #47734: <==uncertain_firing== 36751 (pos)
                    (when (and (not (not_at_c_l1)))
                          (not (Pc_not_secret_b)))

                    ; #52158: <==uncertain_firing== 77434 (pos)
                    (when (and (not (not_at_a_l1)))
                          (not (Ba_not_secret_b)))

                    ; #56215: <==negation-removal== 76029 (pos)
                    (when (and (at_d_l1))
                          (not (Pd_not_secret_b)))

                    ; #57268: <==uncertain_firing== 65285 (pos)
                    (when (and (not (not_at_c_l1)))
                          (not (Bc_not_secret_b)))

                    ; #59894: <==negation-removal== 36751 (pos)
                    (when (and (at_c_l1))
                          (not (Pc_not_secret_b)))

                    ; #70106: <==negation-removal== 85879 (pos)
                    (when (and (at_d_l1))
                          (not (Bd_not_secret_b)))

                    ; #73836: <==uncertain_firing== 40173 (pos)
                    (when (and (not (not_at_b_l1)))
                          (not (Pb_not_secret_b)))

                    ; #77799: <==negation-removal== 40173 (pos)
                    (when (and (at_b_l1))
                          (not (Pb_not_secret_b)))

                    ; #86338: <==negation-removal== 49982 (pos)
                    (when (and (at_b_l1))
                          (not (Bb_not_secret_b)))))

    (:action share_a_b_l2
        :precondition (and (Ba_secret_b)
                           (at_a_l2))
        :effect (and
                    ; #11249: <==closure== 35073 (pos)
                    (when (and (at_a_l2))
                          (Pa_secret_b))

                    ; #19915: origin
                    (when (and (at_b_l2))
                          (Bb_secret_b))

                    ; #35073: origin
                    (when (and (at_a_l2))
                          (Ba_secret_b))

                    ; #44610: <==closure== 72416 (pos)
                    (when (and (at_c_l2))
                          (Pc_secret_b))

                    ; #53454: <==closure== 69245 (pos)
                    (when (and (at_d_l2))
                          (Pd_secret_b))

                    ; #69245: origin
                    (when (and (at_d_l2))
                          (Bd_secret_b))

                    ; #72416: origin
                    (when (and (at_c_l2))
                          (Bc_secret_b))

                    ; #79557: <==closure== 19915 (pos)
                    (when (and (at_b_l2))
                          (Pb_secret_b))

                    ; #14221: <==negation-removal== 11249 (pos)
                    (when (and (at_a_l2))
                          (not (Ba_not_secret_b)))

                    ; #14339: <==negation-removal== 19915 (pos)
                    (when (and (at_b_l2))
                          (not (Pb_not_secret_b)))

                    ; #22461: <==uncertain_firing== 69245 (pos)
                    (when (and (not (not_at_d_l2)))
                          (not (Pd_not_secret_b)))

                    ; #23757: <==negation-removal== 53454 (pos)
                    (when (and (at_d_l2))
                          (not (Bd_not_secret_b)))

                    ; #28915: <==negation-removal== 69245 (pos)
                    (when (and (at_d_l2))
                          (not (Pd_not_secret_b)))

                    ; #31652: <==uncertain_firing== 44610 (pos)
                    (when (and (not (not_at_c_l2)))
                          (not (Bc_not_secret_b)))

                    ; #36397: <==uncertain_firing== 53454 (pos)
                    (when (and (not (not_at_d_l2)))
                          (not (Bd_not_secret_b)))

                    ; #38489: <==negation-removal== 72416 (pos)
                    (when (and (at_c_l2))
                          (not (Pc_not_secret_b)))

                    ; #40408: <==uncertain_firing== 11249 (pos)
                    (when (and (not (not_at_a_l2)))
                          (not (Ba_not_secret_b)))

                    ; #44794: <==uncertain_firing== 19915 (pos)
                    (when (and (not (not_at_b_l2)))
                          (not (Pb_not_secret_b)))

                    ; #55088: <==negation-removal== 35073 (pos)
                    (when (and (at_a_l2))
                          (not (Pa_not_secret_b)))

                    ; #55680: <==uncertain_firing== 72416 (pos)
                    (when (and (not (not_at_c_l2)))
                          (not (Pc_not_secret_b)))

                    ; #61057: <==uncertain_firing== 35073 (pos)
                    (when (and (not (not_at_a_l2)))
                          (not (Pa_not_secret_b)))

                    ; #67298: <==negation-removal== 44610 (pos)
                    (when (and (at_c_l2))
                          (not (Bc_not_secret_b)))

                    ; #76347: <==negation-removal== 79557 (pos)
                    (when (and (at_b_l2))
                          (not (Bb_not_secret_b)))

                    ; #87686: <==uncertain_firing== 79557 (pos)
                    (when (and (not (not_at_b_l2)))
                          (not (Bb_not_secret_b)))))

    (:action share_a_c_l1
        :precondition (and (at_a_l1)
                           (Ba_secret_c))
        :effect (and
                    ; #36751: origin
                    (when (and (at_c_l1))
                          (Bc_secret_c))

                    ; #40173: origin
                    (when (and (at_b_l1))
                          (Bb_secret_c))

                    ; #49982: <==closure== 40173 (pos)
                    (when (and (at_b_l1))
                          (Pb_secret_c))

                    ; #65285: <==closure== 36751 (pos)
                    (when (and (at_c_l1))
                          (Pc_secret_c))

                    ; #74565: origin
                    (when (and (at_a_l1))
                          (Ba_secret_c))

                    ; #76029: origin
                    (when (and (at_d_l1))
                          (Bd_secret_c))

                    ; #77434: <==closure== 74565 (pos)
                    (when (and (at_a_l1))
                          (Pa_secret_c))

                    ; #85879: <==closure== 76029 (pos)
                    (when (and (at_d_l1))
                          (Pd_secret_c))

                    ; #24085: <==negation-removal== 65285 (pos)
                    (when (and (at_c_l1))
                          (not (Bc_not_secret_c)))

                    ; #25681: <==negation-removal== 77434 (pos)
                    (when (and (at_a_l1))
                          (not (Ba_not_secret_c)))

                    ; #35373: <==uncertain_firing== 76029 (pos)
                    (when (and (not (not_at_d_l1)))
                          (not (Pd_not_secret_c)))

                    ; #44889: <==uncertain_firing== 49982 (pos)
                    (when (and (not (not_at_b_l1)))
                          (not (Bb_not_secret_c)))

                    ; #45173: <==uncertain_firing== 74565 (pos)
                    (when (and (not (not_at_a_l1)))
                          (not (Pa_not_secret_c)))

                    ; #47318: <==negation-removal== 74565 (pos)
                    (when (and (at_a_l1))
                          (not (Pa_not_secret_c)))

                    ; #47557: <==uncertain_firing== 85879 (pos)
                    (when (and (not (not_at_d_l1)))
                          (not (Bd_not_secret_c)))

                    ; #47734: <==uncertain_firing== 36751 (pos)
                    (when (and (not (not_at_c_l1)))
                          (not (Pc_not_secret_c)))

                    ; #52158: <==uncertain_firing== 77434 (pos)
                    (when (and (not (not_at_a_l1)))
                          (not (Ba_not_secret_c)))

                    ; #56215: <==negation-removal== 76029 (pos)
                    (when (and (at_d_l1))
                          (not (Pd_not_secret_c)))

                    ; #57268: <==uncertain_firing== 65285 (pos)
                    (when (and (not (not_at_c_l1)))
                          (not (Bc_not_secret_c)))

                    ; #59894: <==negation-removal== 36751 (pos)
                    (when (and (at_c_l1))
                          (not (Pc_not_secret_c)))

                    ; #70106: <==negation-removal== 85879 (pos)
                    (when (and (at_d_l1))
                          (not (Bd_not_secret_c)))

                    ; #73836: <==uncertain_firing== 40173 (pos)
                    (when (and (not (not_at_b_l1)))
                          (not (Pb_not_secret_c)))

                    ; #77799: <==negation-removal== 40173 (pos)
                    (when (and (at_b_l1))
                          (not (Pb_not_secret_c)))

                    ; #86338: <==negation-removal== 49982 (pos)
                    (when (and (at_b_l1))
                          (not (Bb_not_secret_c)))))

    (:action share_a_c_l2
        :precondition (and (Ba_secret_c)
                           (at_a_l2))
        :effect (and
                    ; #11249: <==closure== 35073 (pos)
                    (when (and (at_a_l2))
                          (Pa_secret_c))

                    ; #19915: origin
                    (when (and (at_b_l2))
                          (Bb_secret_c))

                    ; #35073: origin
                    (when (and (at_a_l2))
                          (Ba_secret_c))

                    ; #44610: <==closure== 72416 (pos)
                    (when (and (at_c_l2))
                          (Pc_secret_c))

                    ; #53454: <==closure== 69245 (pos)
                    (when (and (at_d_l2))
                          (Pd_secret_c))

                    ; #69245: origin
                    (when (and (at_d_l2))
                          (Bd_secret_c))

                    ; #72416: origin
                    (when (and (at_c_l2))
                          (Bc_secret_c))

                    ; #79557: <==closure== 19915 (pos)
                    (when (and (at_b_l2))
                          (Pb_secret_c))

                    ; #14221: <==negation-removal== 11249 (pos)
                    (when (and (at_a_l2))
                          (not (Ba_not_secret_c)))

                    ; #14339: <==negation-removal== 19915 (pos)
                    (when (and (at_b_l2))
                          (not (Pb_not_secret_c)))

                    ; #22461: <==uncertain_firing== 69245 (pos)
                    (when (and (not (not_at_d_l2)))
                          (not (Pd_not_secret_c)))

                    ; #23757: <==negation-removal== 53454 (pos)
                    (when (and (at_d_l2))
                          (not (Bd_not_secret_c)))

                    ; #28915: <==negation-removal== 69245 (pos)
                    (when (and (at_d_l2))
                          (not (Pd_not_secret_c)))

                    ; #31652: <==uncertain_firing== 44610 (pos)
                    (when (and (not (not_at_c_l2)))
                          (not (Bc_not_secret_c)))

                    ; #36397: <==uncertain_firing== 53454 (pos)
                    (when (and (not (not_at_d_l2)))
                          (not (Bd_not_secret_c)))

                    ; #38489: <==negation-removal== 72416 (pos)
                    (when (and (at_c_l2))
                          (not (Pc_not_secret_c)))

                    ; #40408: <==uncertain_firing== 11249 (pos)
                    (when (and (not (not_at_a_l2)))
                          (not (Ba_not_secret_c)))

                    ; #44794: <==uncertain_firing== 19915 (pos)
                    (when (and (not (not_at_b_l2)))
                          (not (Pb_not_secret_c)))

                    ; #55088: <==negation-removal== 35073 (pos)
                    (when (and (at_a_l2))
                          (not (Pa_not_secret_c)))

                    ; #55680: <==uncertain_firing== 72416 (pos)
                    (when (and (not (not_at_c_l2)))
                          (not (Pc_not_secret_c)))

                    ; #61057: <==uncertain_firing== 35073 (pos)
                    (when (and (not (not_at_a_l2)))
                          (not (Pa_not_secret_c)))

                    ; #67298: <==negation-removal== 44610 (pos)
                    (when (and (at_c_l2))
                          (not (Bc_not_secret_c)))

                    ; #76347: <==negation-removal== 79557 (pos)
                    (when (and (at_b_l2))
                          (not (Bb_not_secret_c)))

                    ; #87686: <==uncertain_firing== 79557 (pos)
                    (when (and (not (not_at_b_l2)))
                          (not (Bb_not_secret_c)))))

    (:action share_a_d_l1
        :precondition (and (at_a_l1)
                           (Ba_secret_d))
        :effect (and
                    ; #36751: origin
                    (when (and (at_c_l1))
                          (Bc_secret_d))

                    ; #40173: origin
                    (when (and (at_b_l1))
                          (Bb_secret_d))

                    ; #49982: <==closure== 40173 (pos)
                    (when (and (at_b_l1))
                          (Pb_secret_d))

                    ; #65285: <==closure== 36751 (pos)
                    (when (and (at_c_l1))
                          (Pc_secret_d))

                    ; #74565: origin
                    (when (and (at_a_l1))
                          (Ba_secret_d))

                    ; #76029: origin
                    (when (and (at_d_l1))
                          (Bd_secret_d))

                    ; #77434: <==closure== 74565 (pos)
                    (when (and (at_a_l1))
                          (Pa_secret_d))

                    ; #85879: <==closure== 76029 (pos)
                    (when (and (at_d_l1))
                          (Pd_secret_d))

                    ; #24085: <==negation-removal== 65285 (pos)
                    (when (and (at_c_l1))
                          (not (Bc_not_secret_d)))

                    ; #25681: <==negation-removal== 77434 (pos)
                    (when (and (at_a_l1))
                          (not (Ba_not_secret_d)))

                    ; #35373: <==uncertain_firing== 76029 (pos)
                    (when (and (not (not_at_d_l1)))
                          (not (Pd_not_secret_d)))

                    ; #44889: <==uncertain_firing== 49982 (pos)
                    (when (and (not (not_at_b_l1)))
                          (not (Bb_not_secret_d)))

                    ; #45173: <==uncertain_firing== 74565 (pos)
                    (when (and (not (not_at_a_l1)))
                          (not (Pa_not_secret_d)))

                    ; #47318: <==negation-removal== 74565 (pos)
                    (when (and (at_a_l1))
                          (not (Pa_not_secret_d)))

                    ; #47557: <==uncertain_firing== 85879 (pos)
                    (when (and (not (not_at_d_l1)))
                          (not (Bd_not_secret_d)))

                    ; #47734: <==uncertain_firing== 36751 (pos)
                    (when (and (not (not_at_c_l1)))
                          (not (Pc_not_secret_d)))

                    ; #52158: <==uncertain_firing== 77434 (pos)
                    (when (and (not (not_at_a_l1)))
                          (not (Ba_not_secret_d)))

                    ; #56215: <==negation-removal== 76029 (pos)
                    (when (and (at_d_l1))
                          (not (Pd_not_secret_d)))

                    ; #57268: <==uncertain_firing== 65285 (pos)
                    (when (and (not (not_at_c_l1)))
                          (not (Bc_not_secret_d)))

                    ; #59894: <==negation-removal== 36751 (pos)
                    (when (and (at_c_l1))
                          (not (Pc_not_secret_d)))

                    ; #70106: <==negation-removal== 85879 (pos)
                    (when (and (at_d_l1))
                          (not (Bd_not_secret_d)))

                    ; #73836: <==uncertain_firing== 40173 (pos)
                    (when (and (not (not_at_b_l1)))
                          (not (Pb_not_secret_d)))

                    ; #77799: <==negation-removal== 40173 (pos)
                    (when (and (at_b_l1))
                          (not (Pb_not_secret_d)))

                    ; #86338: <==negation-removal== 49982 (pos)
                    (when (and (at_b_l1))
                          (not (Bb_not_secret_d)))))

    (:action share_a_d_l2
        :precondition (and (at_a_l2)
                           (Ba_secret_d))
        :effect (and
                    ; #11249: <==closure== 35073 (pos)
                    (when (and (at_a_l2))
                          (Pa_secret_d))

                    ; #19915: origin
                    (when (and (at_b_l2))
                          (Bb_secret_d))

                    ; #35073: origin
                    (when (and (at_a_l2))
                          (Ba_secret_d))

                    ; #44610: <==closure== 72416 (pos)
                    (when (and (at_c_l2))
                          (Pc_secret_d))

                    ; #53454: <==closure== 69245 (pos)
                    (when (and (at_d_l2))
                          (Pd_secret_d))

                    ; #69245: origin
                    (when (and (at_d_l2))
                          (Bd_secret_d))

                    ; #72416: origin
                    (when (and (at_c_l2))
                          (Bc_secret_d))

                    ; #79557: <==closure== 19915 (pos)
                    (when (and (at_b_l2))
                          (Pb_secret_d))

                    ; #14221: <==negation-removal== 11249 (pos)
                    (when (and (at_a_l2))
                          (not (Ba_not_secret_d)))

                    ; #14339: <==negation-removal== 19915 (pos)
                    (when (and (at_b_l2))
                          (not (Pb_not_secret_d)))

                    ; #22461: <==uncertain_firing== 69245 (pos)
                    (when (and (not (not_at_d_l2)))
                          (not (Pd_not_secret_d)))

                    ; #23757: <==negation-removal== 53454 (pos)
                    (when (and (at_d_l2))
                          (not (Bd_not_secret_d)))

                    ; #28915: <==negation-removal== 69245 (pos)
                    (when (and (at_d_l2))
                          (not (Pd_not_secret_d)))

                    ; #31652: <==uncertain_firing== 44610 (pos)
                    (when (and (not (not_at_c_l2)))
                          (not (Bc_not_secret_d)))

                    ; #36397: <==uncertain_firing== 53454 (pos)
                    (when (and (not (not_at_d_l2)))
                          (not (Bd_not_secret_d)))

                    ; #38489: <==negation-removal== 72416 (pos)
                    (when (and (at_c_l2))
                          (not (Pc_not_secret_d)))

                    ; #40408: <==uncertain_firing== 11249 (pos)
                    (when (and (not (not_at_a_l2)))
                          (not (Ba_not_secret_d)))

                    ; #44794: <==uncertain_firing== 19915 (pos)
                    (when (and (not (not_at_b_l2)))
                          (not (Pb_not_secret_d)))

                    ; #55088: <==negation-removal== 35073 (pos)
                    (when (and (at_a_l2))
                          (not (Pa_not_secret_d)))

                    ; #55680: <==uncertain_firing== 72416 (pos)
                    (when (and (not (not_at_c_l2)))
                          (not (Pc_not_secret_d)))

                    ; #61057: <==uncertain_firing== 35073 (pos)
                    (when (and (not (not_at_a_l2)))
                          (not (Pa_not_secret_d)))

                    ; #67298: <==negation-removal== 44610 (pos)
                    (when (and (at_c_l2))
                          (not (Bc_not_secret_d)))

                    ; #76347: <==negation-removal== 79557 (pos)
                    (when (and (at_b_l2))
                          (not (Bb_not_secret_d)))

                    ; #87686: <==uncertain_firing== 79557 (pos)
                    (when (and (not (not_at_b_l2)))
                          (not (Bb_not_secret_d)))))

    (:action share_b_a_l1
        :precondition (and (at_b_l1)
                           (Bb_secret_a))
        :effect (and
                    ; #36751: origin
                    (when (and (at_c_l1))
                          (Bc_secret_a))

                    ; #40173: origin
                    (when (and (at_b_l1))
                          (Bb_secret_a))

                    ; #49982: <==closure== 40173 (pos)
                    (when (and (at_b_l1))
                          (Pb_secret_a))

                    ; #65285: <==closure== 36751 (pos)
                    (when (and (at_c_l1))
                          (Pc_secret_a))

                    ; #74565: origin
                    (when (and (at_a_l1))
                          (Ba_secret_a))

                    ; #76029: origin
                    (when (and (at_d_l1))
                          (Bd_secret_a))

                    ; #77434: <==closure== 74565 (pos)
                    (when (and (at_a_l1))
                          (Pa_secret_a))

                    ; #85879: <==closure== 76029 (pos)
                    (when (and (at_d_l1))
                          (Pd_secret_a))

                    ; #24085: <==negation-removal== 65285 (pos)
                    (when (and (at_c_l1))
                          (not (Bc_not_secret_a)))

                    ; #25681: <==negation-removal== 77434 (pos)
                    (when (and (at_a_l1))
                          (not (Ba_not_secret_a)))

                    ; #35373: <==uncertain_firing== 76029 (pos)
                    (when (and (not (not_at_d_l1)))
                          (not (Pd_not_secret_a)))

                    ; #44889: <==uncertain_firing== 49982 (pos)
                    (when (and (not (not_at_b_l1)))
                          (not (Bb_not_secret_a)))

                    ; #45173: <==uncertain_firing== 74565 (pos)
                    (when (and (not (not_at_a_l1)))
                          (not (Pa_not_secret_a)))

                    ; #47318: <==negation-removal== 74565 (pos)
                    (when (and (at_a_l1))
                          (not (Pa_not_secret_a)))

                    ; #47557: <==uncertain_firing== 85879 (pos)
                    (when (and (not (not_at_d_l1)))
                          (not (Bd_not_secret_a)))

                    ; #47734: <==uncertain_firing== 36751 (pos)
                    (when (and (not (not_at_c_l1)))
                          (not (Pc_not_secret_a)))

                    ; #52158: <==uncertain_firing== 77434 (pos)
                    (when (and (not (not_at_a_l1)))
                          (not (Ba_not_secret_a)))

                    ; #56215: <==negation-removal== 76029 (pos)
                    (when (and (at_d_l1))
                          (not (Pd_not_secret_a)))

                    ; #57268: <==uncertain_firing== 65285 (pos)
                    (when (and (not (not_at_c_l1)))
                          (not (Bc_not_secret_a)))

                    ; #59894: <==negation-removal== 36751 (pos)
                    (when (and (at_c_l1))
                          (not (Pc_not_secret_a)))

                    ; #70106: <==negation-removal== 85879 (pos)
                    (when (and (at_d_l1))
                          (not (Bd_not_secret_a)))

                    ; #73836: <==uncertain_firing== 40173 (pos)
                    (when (and (not (not_at_b_l1)))
                          (not (Pb_not_secret_a)))

                    ; #77799: <==negation-removal== 40173 (pos)
                    (when (and (at_b_l1))
                          (not (Pb_not_secret_a)))

                    ; #86338: <==negation-removal== 49982 (pos)
                    (when (and (at_b_l1))
                          (not (Bb_not_secret_a)))))

    (:action share_b_a_l2
        :precondition (and (at_b_l2)
                           (Bb_secret_a))
        :effect (and
                    ; #11249: <==closure== 35073 (pos)
                    (when (and (at_a_l2))
                          (Pa_secret_a))

                    ; #19915: origin
                    (when (and (at_b_l2))
                          (Bb_secret_a))

                    ; #35073: origin
                    (when (and (at_a_l2))
                          (Ba_secret_a))

                    ; #44610: <==closure== 72416 (pos)
                    (when (and (at_c_l2))
                          (Pc_secret_a))

                    ; #53454: <==closure== 69245 (pos)
                    (when (and (at_d_l2))
                          (Pd_secret_a))

                    ; #69245: origin
                    (when (and (at_d_l2))
                          (Bd_secret_a))

                    ; #72416: origin
                    (when (and (at_c_l2))
                          (Bc_secret_a))

                    ; #79557: <==closure== 19915 (pos)
                    (when (and (at_b_l2))
                          (Pb_secret_a))

                    ; #14221: <==negation-removal== 11249 (pos)
                    (when (and (at_a_l2))
                          (not (Ba_not_secret_a)))

                    ; #14339: <==negation-removal== 19915 (pos)
                    (when (and (at_b_l2))
                          (not (Pb_not_secret_a)))

                    ; #22461: <==uncertain_firing== 69245 (pos)
                    (when (and (not (not_at_d_l2)))
                          (not (Pd_not_secret_a)))

                    ; #23757: <==negation-removal== 53454 (pos)
                    (when (and (at_d_l2))
                          (not (Bd_not_secret_a)))

                    ; #28915: <==negation-removal== 69245 (pos)
                    (when (and (at_d_l2))
                          (not (Pd_not_secret_a)))

                    ; #31652: <==uncertain_firing== 44610 (pos)
                    (when (and (not (not_at_c_l2)))
                          (not (Bc_not_secret_a)))

                    ; #36397: <==uncertain_firing== 53454 (pos)
                    (when (and (not (not_at_d_l2)))
                          (not (Bd_not_secret_a)))

                    ; #38489: <==negation-removal== 72416 (pos)
                    (when (and (at_c_l2))
                          (not (Pc_not_secret_a)))

                    ; #40408: <==uncertain_firing== 11249 (pos)
                    (when (and (not (not_at_a_l2)))
                          (not (Ba_not_secret_a)))

                    ; #44794: <==uncertain_firing== 19915 (pos)
                    (when (and (not (not_at_b_l2)))
                          (not (Pb_not_secret_a)))

                    ; #55088: <==negation-removal== 35073 (pos)
                    (when (and (at_a_l2))
                          (not (Pa_not_secret_a)))

                    ; #55680: <==uncertain_firing== 72416 (pos)
                    (when (and (not (not_at_c_l2)))
                          (not (Pc_not_secret_a)))

                    ; #61057: <==uncertain_firing== 35073 (pos)
                    (when (and (not (not_at_a_l2)))
                          (not (Pa_not_secret_a)))

                    ; #67298: <==negation-removal== 44610 (pos)
                    (when (and (at_c_l2))
                          (not (Bc_not_secret_a)))

                    ; #76347: <==negation-removal== 79557 (pos)
                    (when (and (at_b_l2))
                          (not (Bb_not_secret_a)))

                    ; #87686: <==uncertain_firing== 79557 (pos)
                    (when (and (not (not_at_b_l2)))
                          (not (Bb_not_secret_a)))))

    (:action share_b_b_l1
        :precondition (and (at_b_l1)
                           (Bb_secret_b))
        :effect (and
                    ; #36751: origin
                    (when (and (at_c_l1))
                          (Bc_secret_b))

                    ; #40173: origin
                    (when (and (at_b_l1))
                          (Bb_secret_b))

                    ; #49982: <==closure== 40173 (pos)
                    (when (and (at_b_l1))
                          (Pb_secret_b))

                    ; #65285: <==closure== 36751 (pos)
                    (when (and (at_c_l1))
                          (Pc_secret_b))

                    ; #74565: origin
                    (when (and (at_a_l1))
                          (Ba_secret_b))

                    ; #76029: origin
                    (when (and (at_d_l1))
                          (Bd_secret_b))

                    ; #77434: <==closure== 74565 (pos)
                    (when (and (at_a_l1))
                          (Pa_secret_b))

                    ; #85879: <==closure== 76029 (pos)
                    (when (and (at_d_l1))
                          (Pd_secret_b))

                    ; #24085: <==negation-removal== 65285 (pos)
                    (when (and (at_c_l1))
                          (not (Bc_not_secret_b)))

                    ; #25681: <==negation-removal== 77434 (pos)
                    (when (and (at_a_l1))
                          (not (Ba_not_secret_b)))

                    ; #35373: <==uncertain_firing== 76029 (pos)
                    (when (and (not (not_at_d_l1)))
                          (not (Pd_not_secret_b)))

                    ; #44889: <==uncertain_firing== 49982 (pos)
                    (when (and (not (not_at_b_l1)))
                          (not (Bb_not_secret_b)))

                    ; #45173: <==uncertain_firing== 74565 (pos)
                    (when (and (not (not_at_a_l1)))
                          (not (Pa_not_secret_b)))

                    ; #47318: <==negation-removal== 74565 (pos)
                    (when (and (at_a_l1))
                          (not (Pa_not_secret_b)))

                    ; #47557: <==uncertain_firing== 85879 (pos)
                    (when (and (not (not_at_d_l1)))
                          (not (Bd_not_secret_b)))

                    ; #47734: <==uncertain_firing== 36751 (pos)
                    (when (and (not (not_at_c_l1)))
                          (not (Pc_not_secret_b)))

                    ; #52158: <==uncertain_firing== 77434 (pos)
                    (when (and (not (not_at_a_l1)))
                          (not (Ba_not_secret_b)))

                    ; #56215: <==negation-removal== 76029 (pos)
                    (when (and (at_d_l1))
                          (not (Pd_not_secret_b)))

                    ; #57268: <==uncertain_firing== 65285 (pos)
                    (when (and (not (not_at_c_l1)))
                          (not (Bc_not_secret_b)))

                    ; #59894: <==negation-removal== 36751 (pos)
                    (when (and (at_c_l1))
                          (not (Pc_not_secret_b)))

                    ; #70106: <==negation-removal== 85879 (pos)
                    (when (and (at_d_l1))
                          (not (Bd_not_secret_b)))

                    ; #73836: <==uncertain_firing== 40173 (pos)
                    (when (and (not (not_at_b_l1)))
                          (not (Pb_not_secret_b)))

                    ; #77799: <==negation-removal== 40173 (pos)
                    (when (and (at_b_l1))
                          (not (Pb_not_secret_b)))

                    ; #86338: <==negation-removal== 49982 (pos)
                    (when (and (at_b_l1))
                          (not (Bb_not_secret_b)))))

    (:action share_b_b_l2
        :precondition (and (at_b_l2)
                           (Bb_secret_b))
        :effect (and
                    ; #11249: <==closure== 35073 (pos)
                    (when (and (at_a_l2))
                          (Pa_secret_b))

                    ; #19915: origin
                    (when (and (at_b_l2))
                          (Bb_secret_b))

                    ; #35073: origin
                    (when (and (at_a_l2))
                          (Ba_secret_b))

                    ; #44610: <==closure== 72416 (pos)
                    (when (and (at_c_l2))
                          (Pc_secret_b))

                    ; #53454: <==closure== 69245 (pos)
                    (when (and (at_d_l2))
                          (Pd_secret_b))

                    ; #69245: origin
                    (when (and (at_d_l2))
                          (Bd_secret_b))

                    ; #72416: origin
                    (when (and (at_c_l2))
                          (Bc_secret_b))

                    ; #79557: <==closure== 19915 (pos)
                    (when (and (at_b_l2))
                          (Pb_secret_b))

                    ; #14221: <==negation-removal== 11249 (pos)
                    (when (and (at_a_l2))
                          (not (Ba_not_secret_b)))

                    ; #14339: <==negation-removal== 19915 (pos)
                    (when (and (at_b_l2))
                          (not (Pb_not_secret_b)))

                    ; #22461: <==uncertain_firing== 69245 (pos)
                    (when (and (not (not_at_d_l2)))
                          (not (Pd_not_secret_b)))

                    ; #23757: <==negation-removal== 53454 (pos)
                    (when (and (at_d_l2))
                          (not (Bd_not_secret_b)))

                    ; #28915: <==negation-removal== 69245 (pos)
                    (when (and (at_d_l2))
                          (not (Pd_not_secret_b)))

                    ; #31652: <==uncertain_firing== 44610 (pos)
                    (when (and (not (not_at_c_l2)))
                          (not (Bc_not_secret_b)))

                    ; #36397: <==uncertain_firing== 53454 (pos)
                    (when (and (not (not_at_d_l2)))
                          (not (Bd_not_secret_b)))

                    ; #38489: <==negation-removal== 72416 (pos)
                    (when (and (at_c_l2))
                          (not (Pc_not_secret_b)))

                    ; #40408: <==uncertain_firing== 11249 (pos)
                    (when (and (not (not_at_a_l2)))
                          (not (Ba_not_secret_b)))

                    ; #44794: <==uncertain_firing== 19915 (pos)
                    (when (and (not (not_at_b_l2)))
                          (not (Pb_not_secret_b)))

                    ; #55088: <==negation-removal== 35073 (pos)
                    (when (and (at_a_l2))
                          (not (Pa_not_secret_b)))

                    ; #55680: <==uncertain_firing== 72416 (pos)
                    (when (and (not (not_at_c_l2)))
                          (not (Pc_not_secret_b)))

                    ; #61057: <==uncertain_firing== 35073 (pos)
                    (when (and (not (not_at_a_l2)))
                          (not (Pa_not_secret_b)))

                    ; #67298: <==negation-removal== 44610 (pos)
                    (when (and (at_c_l2))
                          (not (Bc_not_secret_b)))

                    ; #76347: <==negation-removal== 79557 (pos)
                    (when (and (at_b_l2))
                          (not (Bb_not_secret_b)))

                    ; #87686: <==uncertain_firing== 79557 (pos)
                    (when (and (not (not_at_b_l2)))
                          (not (Bb_not_secret_b)))))

    (:action share_b_c_l1
        :precondition (and (at_b_l1)
                           (Bb_secret_c))
        :effect (and
                    ; #36751: origin
                    (when (and (at_c_l1))
                          (Bc_secret_c))

                    ; #40173: origin
                    (when (and (at_b_l1))
                          (Bb_secret_c))

                    ; #49982: <==closure== 40173 (pos)
                    (when (and (at_b_l1))
                          (Pb_secret_c))

                    ; #65285: <==closure== 36751 (pos)
                    (when (and (at_c_l1))
                          (Pc_secret_c))

                    ; #74565: origin
                    (when (and (at_a_l1))
                          (Ba_secret_c))

                    ; #76029: origin
                    (when (and (at_d_l1))
                          (Bd_secret_c))

                    ; #77434: <==closure== 74565 (pos)
                    (when (and (at_a_l1))
                          (Pa_secret_c))

                    ; #85879: <==closure== 76029 (pos)
                    (when (and (at_d_l1))
                          (Pd_secret_c))

                    ; #24085: <==negation-removal== 65285 (pos)
                    (when (and (at_c_l1))
                          (not (Bc_not_secret_c)))

                    ; #25681: <==negation-removal== 77434 (pos)
                    (when (and (at_a_l1))
                          (not (Ba_not_secret_c)))

                    ; #35373: <==uncertain_firing== 76029 (pos)
                    (when (and (not (not_at_d_l1)))
                          (not (Pd_not_secret_c)))

                    ; #44889: <==uncertain_firing== 49982 (pos)
                    (when (and (not (not_at_b_l1)))
                          (not (Bb_not_secret_c)))

                    ; #45173: <==uncertain_firing== 74565 (pos)
                    (when (and (not (not_at_a_l1)))
                          (not (Pa_not_secret_c)))

                    ; #47318: <==negation-removal== 74565 (pos)
                    (when (and (at_a_l1))
                          (not (Pa_not_secret_c)))

                    ; #47557: <==uncertain_firing== 85879 (pos)
                    (when (and (not (not_at_d_l1)))
                          (not (Bd_not_secret_c)))

                    ; #47734: <==uncertain_firing== 36751 (pos)
                    (when (and (not (not_at_c_l1)))
                          (not (Pc_not_secret_c)))

                    ; #52158: <==uncertain_firing== 77434 (pos)
                    (when (and (not (not_at_a_l1)))
                          (not (Ba_not_secret_c)))

                    ; #56215: <==negation-removal== 76029 (pos)
                    (when (and (at_d_l1))
                          (not (Pd_not_secret_c)))

                    ; #57268: <==uncertain_firing== 65285 (pos)
                    (when (and (not (not_at_c_l1)))
                          (not (Bc_not_secret_c)))

                    ; #59894: <==negation-removal== 36751 (pos)
                    (when (and (at_c_l1))
                          (not (Pc_not_secret_c)))

                    ; #70106: <==negation-removal== 85879 (pos)
                    (when (and (at_d_l1))
                          (not (Bd_not_secret_c)))

                    ; #73836: <==uncertain_firing== 40173 (pos)
                    (when (and (not (not_at_b_l1)))
                          (not (Pb_not_secret_c)))

                    ; #77799: <==negation-removal== 40173 (pos)
                    (when (and (at_b_l1))
                          (not (Pb_not_secret_c)))

                    ; #86338: <==negation-removal== 49982 (pos)
                    (when (and (at_b_l1))
                          (not (Bb_not_secret_c)))))

    (:action share_b_c_l2
        :precondition (and (at_b_l2)
                           (Bb_secret_c))
        :effect (and
                    ; #11249: <==closure== 35073 (pos)
                    (when (and (at_a_l2))
                          (Pa_secret_c))

                    ; #19915: origin
                    (when (and (at_b_l2))
                          (Bb_secret_c))

                    ; #35073: origin
                    (when (and (at_a_l2))
                          (Ba_secret_c))

                    ; #44610: <==closure== 72416 (pos)
                    (when (and (at_c_l2))
                          (Pc_secret_c))

                    ; #53454: <==closure== 69245 (pos)
                    (when (and (at_d_l2))
                          (Pd_secret_c))

                    ; #69245: origin
                    (when (and (at_d_l2))
                          (Bd_secret_c))

                    ; #72416: origin
                    (when (and (at_c_l2))
                          (Bc_secret_c))

                    ; #79557: <==closure== 19915 (pos)
                    (when (and (at_b_l2))
                          (Pb_secret_c))

                    ; #14221: <==negation-removal== 11249 (pos)
                    (when (and (at_a_l2))
                          (not (Ba_not_secret_c)))

                    ; #14339: <==negation-removal== 19915 (pos)
                    (when (and (at_b_l2))
                          (not (Pb_not_secret_c)))

                    ; #22461: <==uncertain_firing== 69245 (pos)
                    (when (and (not (not_at_d_l2)))
                          (not (Pd_not_secret_c)))

                    ; #23757: <==negation-removal== 53454 (pos)
                    (when (and (at_d_l2))
                          (not (Bd_not_secret_c)))

                    ; #28915: <==negation-removal== 69245 (pos)
                    (when (and (at_d_l2))
                          (not (Pd_not_secret_c)))

                    ; #31652: <==uncertain_firing== 44610 (pos)
                    (when (and (not (not_at_c_l2)))
                          (not (Bc_not_secret_c)))

                    ; #36397: <==uncertain_firing== 53454 (pos)
                    (when (and (not (not_at_d_l2)))
                          (not (Bd_not_secret_c)))

                    ; #38489: <==negation-removal== 72416 (pos)
                    (when (and (at_c_l2))
                          (not (Pc_not_secret_c)))

                    ; #40408: <==uncertain_firing== 11249 (pos)
                    (when (and (not (not_at_a_l2)))
                          (not (Ba_not_secret_c)))

                    ; #44794: <==uncertain_firing== 19915 (pos)
                    (when (and (not (not_at_b_l2)))
                          (not (Pb_not_secret_c)))

                    ; #55088: <==negation-removal== 35073 (pos)
                    (when (and (at_a_l2))
                          (not (Pa_not_secret_c)))

                    ; #55680: <==uncertain_firing== 72416 (pos)
                    (when (and (not (not_at_c_l2)))
                          (not (Pc_not_secret_c)))

                    ; #61057: <==uncertain_firing== 35073 (pos)
                    (when (and (not (not_at_a_l2)))
                          (not (Pa_not_secret_c)))

                    ; #67298: <==negation-removal== 44610 (pos)
                    (when (and (at_c_l2))
                          (not (Bc_not_secret_c)))

                    ; #76347: <==negation-removal== 79557 (pos)
                    (when (and (at_b_l2))
                          (not (Bb_not_secret_c)))

                    ; #87686: <==uncertain_firing== 79557 (pos)
                    (when (and (not (not_at_b_l2)))
                          (not (Bb_not_secret_c)))))

    (:action share_b_d_l1
        :precondition (and (at_b_l1)
                           (Bb_secret_d))
        :effect (and
                    ; #36751: origin
                    (when (and (at_c_l1))
                          (Bc_secret_d))

                    ; #40173: origin
                    (when (and (at_b_l1))
                          (Bb_secret_d))

                    ; #49982: <==closure== 40173 (pos)
                    (when (and (at_b_l1))
                          (Pb_secret_d))

                    ; #65285: <==closure== 36751 (pos)
                    (when (and (at_c_l1))
                          (Pc_secret_d))

                    ; #74565: origin
                    (when (and (at_a_l1))
                          (Ba_secret_d))

                    ; #76029: origin
                    (when (and (at_d_l1))
                          (Bd_secret_d))

                    ; #77434: <==closure== 74565 (pos)
                    (when (and (at_a_l1))
                          (Pa_secret_d))

                    ; #85879: <==closure== 76029 (pos)
                    (when (and (at_d_l1))
                          (Pd_secret_d))

                    ; #24085: <==negation-removal== 65285 (pos)
                    (when (and (at_c_l1))
                          (not (Bc_not_secret_d)))

                    ; #25681: <==negation-removal== 77434 (pos)
                    (when (and (at_a_l1))
                          (not (Ba_not_secret_d)))

                    ; #35373: <==uncertain_firing== 76029 (pos)
                    (when (and (not (not_at_d_l1)))
                          (not (Pd_not_secret_d)))

                    ; #44889: <==uncertain_firing== 49982 (pos)
                    (when (and (not (not_at_b_l1)))
                          (not (Bb_not_secret_d)))

                    ; #45173: <==uncertain_firing== 74565 (pos)
                    (when (and (not (not_at_a_l1)))
                          (not (Pa_not_secret_d)))

                    ; #47318: <==negation-removal== 74565 (pos)
                    (when (and (at_a_l1))
                          (not (Pa_not_secret_d)))

                    ; #47557: <==uncertain_firing== 85879 (pos)
                    (when (and (not (not_at_d_l1)))
                          (not (Bd_not_secret_d)))

                    ; #47734: <==uncertain_firing== 36751 (pos)
                    (when (and (not (not_at_c_l1)))
                          (not (Pc_not_secret_d)))

                    ; #52158: <==uncertain_firing== 77434 (pos)
                    (when (and (not (not_at_a_l1)))
                          (not (Ba_not_secret_d)))

                    ; #56215: <==negation-removal== 76029 (pos)
                    (when (and (at_d_l1))
                          (not (Pd_not_secret_d)))

                    ; #57268: <==uncertain_firing== 65285 (pos)
                    (when (and (not (not_at_c_l1)))
                          (not (Bc_not_secret_d)))

                    ; #59894: <==negation-removal== 36751 (pos)
                    (when (and (at_c_l1))
                          (not (Pc_not_secret_d)))

                    ; #70106: <==negation-removal== 85879 (pos)
                    (when (and (at_d_l1))
                          (not (Bd_not_secret_d)))

                    ; #73836: <==uncertain_firing== 40173 (pos)
                    (when (and (not (not_at_b_l1)))
                          (not (Pb_not_secret_d)))

                    ; #77799: <==negation-removal== 40173 (pos)
                    (when (and (at_b_l1))
                          (not (Pb_not_secret_d)))

                    ; #86338: <==negation-removal== 49982 (pos)
                    (when (and (at_b_l1))
                          (not (Bb_not_secret_d)))))

    (:action share_b_d_l2
        :precondition (and (at_b_l2)
                           (Bb_secret_d))
        :effect (and
                    ; #11249: <==closure== 35073 (pos)
                    (when (and (at_a_l2))
                          (Pa_secret_d))

                    ; #19915: origin
                    (when (and (at_b_l2))
                          (Bb_secret_d))

                    ; #35073: origin
                    (when (and (at_a_l2))
                          (Ba_secret_d))

                    ; #44610: <==closure== 72416 (pos)
                    (when (and (at_c_l2))
                          (Pc_secret_d))

                    ; #53454: <==closure== 69245 (pos)
                    (when (and (at_d_l2))
                          (Pd_secret_d))

                    ; #69245: origin
                    (when (and (at_d_l2))
                          (Bd_secret_d))

                    ; #72416: origin
                    (when (and (at_c_l2))
                          (Bc_secret_d))

                    ; #79557: <==closure== 19915 (pos)
                    (when (and (at_b_l2))
                          (Pb_secret_d))

                    ; #14221: <==negation-removal== 11249 (pos)
                    (when (and (at_a_l2))
                          (not (Ba_not_secret_d)))

                    ; #14339: <==negation-removal== 19915 (pos)
                    (when (and (at_b_l2))
                          (not (Pb_not_secret_d)))

                    ; #22461: <==uncertain_firing== 69245 (pos)
                    (when (and (not (not_at_d_l2)))
                          (not (Pd_not_secret_d)))

                    ; #23757: <==negation-removal== 53454 (pos)
                    (when (and (at_d_l2))
                          (not (Bd_not_secret_d)))

                    ; #28915: <==negation-removal== 69245 (pos)
                    (when (and (at_d_l2))
                          (not (Pd_not_secret_d)))

                    ; #31652: <==uncertain_firing== 44610 (pos)
                    (when (and (not (not_at_c_l2)))
                          (not (Bc_not_secret_d)))

                    ; #36397: <==uncertain_firing== 53454 (pos)
                    (when (and (not (not_at_d_l2)))
                          (not (Bd_not_secret_d)))

                    ; #38489: <==negation-removal== 72416 (pos)
                    (when (and (at_c_l2))
                          (not (Pc_not_secret_d)))

                    ; #40408: <==uncertain_firing== 11249 (pos)
                    (when (and (not (not_at_a_l2)))
                          (not (Ba_not_secret_d)))

                    ; #44794: <==uncertain_firing== 19915 (pos)
                    (when (and (not (not_at_b_l2)))
                          (not (Pb_not_secret_d)))

                    ; #55088: <==negation-removal== 35073 (pos)
                    (when (and (at_a_l2))
                          (not (Pa_not_secret_d)))

                    ; #55680: <==uncertain_firing== 72416 (pos)
                    (when (and (not (not_at_c_l2)))
                          (not (Pc_not_secret_d)))

                    ; #61057: <==uncertain_firing== 35073 (pos)
                    (when (and (not (not_at_a_l2)))
                          (not (Pa_not_secret_d)))

                    ; #67298: <==negation-removal== 44610 (pos)
                    (when (and (at_c_l2))
                          (not (Bc_not_secret_d)))

                    ; #76347: <==negation-removal== 79557 (pos)
                    (when (and (at_b_l2))
                          (not (Bb_not_secret_d)))

                    ; #87686: <==uncertain_firing== 79557 (pos)
                    (when (and (not (not_at_b_l2)))
                          (not (Bb_not_secret_d)))))

    (:action share_c_a_l1
        :precondition (and (Bc_secret_a)
                           (at_c_l1))
        :effect (and
                    ; #36751: origin
                    (when (and (at_c_l1))
                          (Bc_secret_a))

                    ; #40173: origin
                    (when (and (at_b_l1))
                          (Bb_secret_a))

                    ; #49982: <==closure== 40173 (pos)
                    (when (and (at_b_l1))
                          (Pb_secret_a))

                    ; #65285: <==closure== 36751 (pos)
                    (when (and (at_c_l1))
                          (Pc_secret_a))

                    ; #74565: origin
                    (when (and (at_a_l1))
                          (Ba_secret_a))

                    ; #76029: origin
                    (when (and (at_d_l1))
                          (Bd_secret_a))

                    ; #77434: <==closure== 74565 (pos)
                    (when (and (at_a_l1))
                          (Pa_secret_a))

                    ; #85879: <==closure== 76029 (pos)
                    (when (and (at_d_l1))
                          (Pd_secret_a))

                    ; #24085: <==negation-removal== 65285 (pos)
                    (when (and (at_c_l1))
                          (not (Bc_not_secret_a)))

                    ; #25681: <==negation-removal== 77434 (pos)
                    (when (and (at_a_l1))
                          (not (Ba_not_secret_a)))

                    ; #35373: <==uncertain_firing== 76029 (pos)
                    (when (and (not (not_at_d_l1)))
                          (not (Pd_not_secret_a)))

                    ; #44889: <==uncertain_firing== 49982 (pos)
                    (when (and (not (not_at_b_l1)))
                          (not (Bb_not_secret_a)))

                    ; #45173: <==uncertain_firing== 74565 (pos)
                    (when (and (not (not_at_a_l1)))
                          (not (Pa_not_secret_a)))

                    ; #47318: <==negation-removal== 74565 (pos)
                    (when (and (at_a_l1))
                          (not (Pa_not_secret_a)))

                    ; #47557: <==uncertain_firing== 85879 (pos)
                    (when (and (not (not_at_d_l1)))
                          (not (Bd_not_secret_a)))

                    ; #47734: <==uncertain_firing== 36751 (pos)
                    (when (and (not (not_at_c_l1)))
                          (not (Pc_not_secret_a)))

                    ; #52158: <==uncertain_firing== 77434 (pos)
                    (when (and (not (not_at_a_l1)))
                          (not (Ba_not_secret_a)))

                    ; #56215: <==negation-removal== 76029 (pos)
                    (when (and (at_d_l1))
                          (not (Pd_not_secret_a)))

                    ; #57268: <==uncertain_firing== 65285 (pos)
                    (when (and (not (not_at_c_l1)))
                          (not (Bc_not_secret_a)))

                    ; #59894: <==negation-removal== 36751 (pos)
                    (when (and (at_c_l1))
                          (not (Pc_not_secret_a)))

                    ; #70106: <==negation-removal== 85879 (pos)
                    (when (and (at_d_l1))
                          (not (Bd_not_secret_a)))

                    ; #73836: <==uncertain_firing== 40173 (pos)
                    (when (and (not (not_at_b_l1)))
                          (not (Pb_not_secret_a)))

                    ; #77799: <==negation-removal== 40173 (pos)
                    (when (and (at_b_l1))
                          (not (Pb_not_secret_a)))

                    ; #86338: <==negation-removal== 49982 (pos)
                    (when (and (at_b_l1))
                          (not (Bb_not_secret_a)))))

    (:action share_c_a_l2
        :precondition (and (Bc_secret_a)
                           (at_c_l2))
        :effect (and
                    ; #11249: <==closure== 35073 (pos)
                    (when (and (at_a_l2))
                          (Pa_secret_a))

                    ; #19915: origin
                    (when (and (at_b_l2))
                          (Bb_secret_a))

                    ; #35073: origin
                    (when (and (at_a_l2))
                          (Ba_secret_a))

                    ; #44610: <==closure== 72416 (pos)
                    (when (and (at_c_l2))
                          (Pc_secret_a))

                    ; #53454: <==closure== 69245 (pos)
                    (when (and (at_d_l2))
                          (Pd_secret_a))

                    ; #69245: origin
                    (when (and (at_d_l2))
                          (Bd_secret_a))

                    ; #72416: origin
                    (when (and (at_c_l2))
                          (Bc_secret_a))

                    ; #79557: <==closure== 19915 (pos)
                    (when (and (at_b_l2))
                          (Pb_secret_a))

                    ; #14221: <==negation-removal== 11249 (pos)
                    (when (and (at_a_l2))
                          (not (Ba_not_secret_a)))

                    ; #14339: <==negation-removal== 19915 (pos)
                    (when (and (at_b_l2))
                          (not (Pb_not_secret_a)))

                    ; #22461: <==uncertain_firing== 69245 (pos)
                    (when (and (not (not_at_d_l2)))
                          (not (Pd_not_secret_a)))

                    ; #23757: <==negation-removal== 53454 (pos)
                    (when (and (at_d_l2))
                          (not (Bd_not_secret_a)))

                    ; #28915: <==negation-removal== 69245 (pos)
                    (when (and (at_d_l2))
                          (not (Pd_not_secret_a)))

                    ; #31652: <==uncertain_firing== 44610 (pos)
                    (when (and (not (not_at_c_l2)))
                          (not (Bc_not_secret_a)))

                    ; #36397: <==uncertain_firing== 53454 (pos)
                    (when (and (not (not_at_d_l2)))
                          (not (Bd_not_secret_a)))

                    ; #38489: <==negation-removal== 72416 (pos)
                    (when (and (at_c_l2))
                          (not (Pc_not_secret_a)))

                    ; #40408: <==uncertain_firing== 11249 (pos)
                    (when (and (not (not_at_a_l2)))
                          (not (Ba_not_secret_a)))

                    ; #44794: <==uncertain_firing== 19915 (pos)
                    (when (and (not (not_at_b_l2)))
                          (not (Pb_not_secret_a)))

                    ; #55088: <==negation-removal== 35073 (pos)
                    (when (and (at_a_l2))
                          (not (Pa_not_secret_a)))

                    ; #55680: <==uncertain_firing== 72416 (pos)
                    (when (and (not (not_at_c_l2)))
                          (not (Pc_not_secret_a)))

                    ; #61057: <==uncertain_firing== 35073 (pos)
                    (when (and (not (not_at_a_l2)))
                          (not (Pa_not_secret_a)))

                    ; #67298: <==negation-removal== 44610 (pos)
                    (when (and (at_c_l2))
                          (not (Bc_not_secret_a)))

                    ; #76347: <==negation-removal== 79557 (pos)
                    (when (and (at_b_l2))
                          (not (Bb_not_secret_a)))

                    ; #87686: <==uncertain_firing== 79557 (pos)
                    (when (and (not (not_at_b_l2)))
                          (not (Bb_not_secret_a)))))

    (:action share_c_b_l1
        :precondition (and (at_c_l1)
                           (Bc_secret_b))
        :effect (and
                    ; #36751: origin
                    (when (and (at_c_l1))
                          (Bc_secret_b))

                    ; #40173: origin
                    (when (and (at_b_l1))
                          (Bb_secret_b))

                    ; #49982: <==closure== 40173 (pos)
                    (when (and (at_b_l1))
                          (Pb_secret_b))

                    ; #65285: <==closure== 36751 (pos)
                    (when (and (at_c_l1))
                          (Pc_secret_b))

                    ; #74565: origin
                    (when (and (at_a_l1))
                          (Ba_secret_b))

                    ; #76029: origin
                    (when (and (at_d_l1))
                          (Bd_secret_b))

                    ; #77434: <==closure== 74565 (pos)
                    (when (and (at_a_l1))
                          (Pa_secret_b))

                    ; #85879: <==closure== 76029 (pos)
                    (when (and (at_d_l1))
                          (Pd_secret_b))

                    ; #24085: <==negation-removal== 65285 (pos)
                    (when (and (at_c_l1))
                          (not (Bc_not_secret_b)))

                    ; #25681: <==negation-removal== 77434 (pos)
                    (when (and (at_a_l1))
                          (not (Ba_not_secret_b)))

                    ; #35373: <==uncertain_firing== 76029 (pos)
                    (when (and (not (not_at_d_l1)))
                          (not (Pd_not_secret_b)))

                    ; #44889: <==uncertain_firing== 49982 (pos)
                    (when (and (not (not_at_b_l1)))
                          (not (Bb_not_secret_b)))

                    ; #45173: <==uncertain_firing== 74565 (pos)
                    (when (and (not (not_at_a_l1)))
                          (not (Pa_not_secret_b)))

                    ; #47318: <==negation-removal== 74565 (pos)
                    (when (and (at_a_l1))
                          (not (Pa_not_secret_b)))

                    ; #47557: <==uncertain_firing== 85879 (pos)
                    (when (and (not (not_at_d_l1)))
                          (not (Bd_not_secret_b)))

                    ; #47734: <==uncertain_firing== 36751 (pos)
                    (when (and (not (not_at_c_l1)))
                          (not (Pc_not_secret_b)))

                    ; #52158: <==uncertain_firing== 77434 (pos)
                    (when (and (not (not_at_a_l1)))
                          (not (Ba_not_secret_b)))

                    ; #56215: <==negation-removal== 76029 (pos)
                    (when (and (at_d_l1))
                          (not (Pd_not_secret_b)))

                    ; #57268: <==uncertain_firing== 65285 (pos)
                    (when (and (not (not_at_c_l1)))
                          (not (Bc_not_secret_b)))

                    ; #59894: <==negation-removal== 36751 (pos)
                    (when (and (at_c_l1))
                          (not (Pc_not_secret_b)))

                    ; #70106: <==negation-removal== 85879 (pos)
                    (when (and (at_d_l1))
                          (not (Bd_not_secret_b)))

                    ; #73836: <==uncertain_firing== 40173 (pos)
                    (when (and (not (not_at_b_l1)))
                          (not (Pb_not_secret_b)))

                    ; #77799: <==negation-removal== 40173 (pos)
                    (when (and (at_b_l1))
                          (not (Pb_not_secret_b)))

                    ; #86338: <==negation-removal== 49982 (pos)
                    (when (and (at_b_l1))
                          (not (Bb_not_secret_b)))))

    (:action share_c_b_l2
        :precondition (and (at_c_l2)
                           (Bc_secret_b))
        :effect (and
                    ; #11249: <==closure== 35073 (pos)
                    (when (and (at_a_l2))
                          (Pa_secret_b))

                    ; #19915: origin
                    (when (and (at_b_l2))
                          (Bb_secret_b))

                    ; #35073: origin
                    (when (and (at_a_l2))
                          (Ba_secret_b))

                    ; #44610: <==closure== 72416 (pos)
                    (when (and (at_c_l2))
                          (Pc_secret_b))

                    ; #53454: <==closure== 69245 (pos)
                    (when (and (at_d_l2))
                          (Pd_secret_b))

                    ; #69245: origin
                    (when (and (at_d_l2))
                          (Bd_secret_b))

                    ; #72416: origin
                    (when (and (at_c_l2))
                          (Bc_secret_b))

                    ; #79557: <==closure== 19915 (pos)
                    (when (and (at_b_l2))
                          (Pb_secret_b))

                    ; #14221: <==negation-removal== 11249 (pos)
                    (when (and (at_a_l2))
                          (not (Ba_not_secret_b)))

                    ; #14339: <==negation-removal== 19915 (pos)
                    (when (and (at_b_l2))
                          (not (Pb_not_secret_b)))

                    ; #22461: <==uncertain_firing== 69245 (pos)
                    (when (and (not (not_at_d_l2)))
                          (not (Pd_not_secret_b)))

                    ; #23757: <==negation-removal== 53454 (pos)
                    (when (and (at_d_l2))
                          (not (Bd_not_secret_b)))

                    ; #28915: <==negation-removal== 69245 (pos)
                    (when (and (at_d_l2))
                          (not (Pd_not_secret_b)))

                    ; #31652: <==uncertain_firing== 44610 (pos)
                    (when (and (not (not_at_c_l2)))
                          (not (Bc_not_secret_b)))

                    ; #36397: <==uncertain_firing== 53454 (pos)
                    (when (and (not (not_at_d_l2)))
                          (not (Bd_not_secret_b)))

                    ; #38489: <==negation-removal== 72416 (pos)
                    (when (and (at_c_l2))
                          (not (Pc_not_secret_b)))

                    ; #40408: <==uncertain_firing== 11249 (pos)
                    (when (and (not (not_at_a_l2)))
                          (not (Ba_not_secret_b)))

                    ; #44794: <==uncertain_firing== 19915 (pos)
                    (when (and (not (not_at_b_l2)))
                          (not (Pb_not_secret_b)))

                    ; #55088: <==negation-removal== 35073 (pos)
                    (when (and (at_a_l2))
                          (not (Pa_not_secret_b)))

                    ; #55680: <==uncertain_firing== 72416 (pos)
                    (when (and (not (not_at_c_l2)))
                          (not (Pc_not_secret_b)))

                    ; #61057: <==uncertain_firing== 35073 (pos)
                    (when (and (not (not_at_a_l2)))
                          (not (Pa_not_secret_b)))

                    ; #67298: <==negation-removal== 44610 (pos)
                    (when (and (at_c_l2))
                          (not (Bc_not_secret_b)))

                    ; #76347: <==negation-removal== 79557 (pos)
                    (when (and (at_b_l2))
                          (not (Bb_not_secret_b)))

                    ; #87686: <==uncertain_firing== 79557 (pos)
                    (when (and (not (not_at_b_l2)))
                          (not (Bb_not_secret_b)))))

    (:action share_c_c_l1
        :precondition (and (at_c_l1)
                           (Bc_secret_c))
        :effect (and
                    ; #36751: origin
                    (when (and (at_c_l1))
                          (Bc_secret_c))

                    ; #40173: origin
                    (when (and (at_b_l1))
                          (Bb_secret_c))

                    ; #49982: <==closure== 40173 (pos)
                    (when (and (at_b_l1))
                          (Pb_secret_c))

                    ; #65285: <==closure== 36751 (pos)
                    (when (and (at_c_l1))
                          (Pc_secret_c))

                    ; #74565: origin
                    (when (and (at_a_l1))
                          (Ba_secret_c))

                    ; #76029: origin
                    (when (and (at_d_l1))
                          (Bd_secret_c))

                    ; #77434: <==closure== 74565 (pos)
                    (when (and (at_a_l1))
                          (Pa_secret_c))

                    ; #85879: <==closure== 76029 (pos)
                    (when (and (at_d_l1))
                          (Pd_secret_c))

                    ; #24085: <==negation-removal== 65285 (pos)
                    (when (and (at_c_l1))
                          (not (Bc_not_secret_c)))

                    ; #25681: <==negation-removal== 77434 (pos)
                    (when (and (at_a_l1))
                          (not (Ba_not_secret_c)))

                    ; #35373: <==uncertain_firing== 76029 (pos)
                    (when (and (not (not_at_d_l1)))
                          (not (Pd_not_secret_c)))

                    ; #44889: <==uncertain_firing== 49982 (pos)
                    (when (and (not (not_at_b_l1)))
                          (not (Bb_not_secret_c)))

                    ; #45173: <==uncertain_firing== 74565 (pos)
                    (when (and (not (not_at_a_l1)))
                          (not (Pa_not_secret_c)))

                    ; #47318: <==negation-removal== 74565 (pos)
                    (when (and (at_a_l1))
                          (not (Pa_not_secret_c)))

                    ; #47557: <==uncertain_firing== 85879 (pos)
                    (when (and (not (not_at_d_l1)))
                          (not (Bd_not_secret_c)))

                    ; #47734: <==uncertain_firing== 36751 (pos)
                    (when (and (not (not_at_c_l1)))
                          (not (Pc_not_secret_c)))

                    ; #52158: <==uncertain_firing== 77434 (pos)
                    (when (and (not (not_at_a_l1)))
                          (not (Ba_not_secret_c)))

                    ; #56215: <==negation-removal== 76029 (pos)
                    (when (and (at_d_l1))
                          (not (Pd_not_secret_c)))

                    ; #57268: <==uncertain_firing== 65285 (pos)
                    (when (and (not (not_at_c_l1)))
                          (not (Bc_not_secret_c)))

                    ; #59894: <==negation-removal== 36751 (pos)
                    (when (and (at_c_l1))
                          (not (Pc_not_secret_c)))

                    ; #70106: <==negation-removal== 85879 (pos)
                    (when (and (at_d_l1))
                          (not (Bd_not_secret_c)))

                    ; #73836: <==uncertain_firing== 40173 (pos)
                    (when (and (not (not_at_b_l1)))
                          (not (Pb_not_secret_c)))

                    ; #77799: <==negation-removal== 40173 (pos)
                    (when (and (at_b_l1))
                          (not (Pb_not_secret_c)))

                    ; #86338: <==negation-removal== 49982 (pos)
                    (when (and (at_b_l1))
                          (not (Bb_not_secret_c)))))

    (:action share_c_c_l2
        :precondition (and (at_c_l2)
                           (Bc_secret_c))
        :effect (and
                    ; #11249: <==closure== 35073 (pos)
                    (when (and (at_a_l2))
                          (Pa_secret_c))

                    ; #19915: origin
                    (when (and (at_b_l2))
                          (Bb_secret_c))

                    ; #35073: origin
                    (when (and (at_a_l2))
                          (Ba_secret_c))

                    ; #44610: <==closure== 72416 (pos)
                    (when (and (at_c_l2))
                          (Pc_secret_c))

                    ; #53454: <==closure== 69245 (pos)
                    (when (and (at_d_l2))
                          (Pd_secret_c))

                    ; #69245: origin
                    (when (and (at_d_l2))
                          (Bd_secret_c))

                    ; #72416: origin
                    (when (and (at_c_l2))
                          (Bc_secret_c))

                    ; #79557: <==closure== 19915 (pos)
                    (when (and (at_b_l2))
                          (Pb_secret_c))

                    ; #14221: <==negation-removal== 11249 (pos)
                    (when (and (at_a_l2))
                          (not (Ba_not_secret_c)))

                    ; #14339: <==negation-removal== 19915 (pos)
                    (when (and (at_b_l2))
                          (not (Pb_not_secret_c)))

                    ; #22461: <==uncertain_firing== 69245 (pos)
                    (when (and (not (not_at_d_l2)))
                          (not (Pd_not_secret_c)))

                    ; #23757: <==negation-removal== 53454 (pos)
                    (when (and (at_d_l2))
                          (not (Bd_not_secret_c)))

                    ; #28915: <==negation-removal== 69245 (pos)
                    (when (and (at_d_l2))
                          (not (Pd_not_secret_c)))

                    ; #31652: <==uncertain_firing== 44610 (pos)
                    (when (and (not (not_at_c_l2)))
                          (not (Bc_not_secret_c)))

                    ; #36397: <==uncertain_firing== 53454 (pos)
                    (when (and (not (not_at_d_l2)))
                          (not (Bd_not_secret_c)))

                    ; #38489: <==negation-removal== 72416 (pos)
                    (when (and (at_c_l2))
                          (not (Pc_not_secret_c)))

                    ; #40408: <==uncertain_firing== 11249 (pos)
                    (when (and (not (not_at_a_l2)))
                          (not (Ba_not_secret_c)))

                    ; #44794: <==uncertain_firing== 19915 (pos)
                    (when (and (not (not_at_b_l2)))
                          (not (Pb_not_secret_c)))

                    ; #55088: <==negation-removal== 35073 (pos)
                    (when (and (at_a_l2))
                          (not (Pa_not_secret_c)))

                    ; #55680: <==uncertain_firing== 72416 (pos)
                    (when (and (not (not_at_c_l2)))
                          (not (Pc_not_secret_c)))

                    ; #61057: <==uncertain_firing== 35073 (pos)
                    (when (and (not (not_at_a_l2)))
                          (not (Pa_not_secret_c)))

                    ; #67298: <==negation-removal== 44610 (pos)
                    (when (and (at_c_l2))
                          (not (Bc_not_secret_c)))

                    ; #76347: <==negation-removal== 79557 (pos)
                    (when (and (at_b_l2))
                          (not (Bb_not_secret_c)))

                    ; #87686: <==uncertain_firing== 79557 (pos)
                    (when (and (not (not_at_b_l2)))
                          (not (Bb_not_secret_c)))))

    (:action share_c_d_l1
        :precondition (and (at_c_l1)
                           (Bc_secret_d))
        :effect (and
                    ; #36751: origin
                    (when (and (at_c_l1))
                          (Bc_secret_d))

                    ; #40173: origin
                    (when (and (at_b_l1))
                          (Bb_secret_d))

                    ; #49982: <==closure== 40173 (pos)
                    (when (and (at_b_l1))
                          (Pb_secret_d))

                    ; #65285: <==closure== 36751 (pos)
                    (when (and (at_c_l1))
                          (Pc_secret_d))

                    ; #74565: origin
                    (when (and (at_a_l1))
                          (Ba_secret_d))

                    ; #76029: origin
                    (when (and (at_d_l1))
                          (Bd_secret_d))

                    ; #77434: <==closure== 74565 (pos)
                    (when (and (at_a_l1))
                          (Pa_secret_d))

                    ; #85879: <==closure== 76029 (pos)
                    (when (and (at_d_l1))
                          (Pd_secret_d))

                    ; #24085: <==negation-removal== 65285 (pos)
                    (when (and (at_c_l1))
                          (not (Bc_not_secret_d)))

                    ; #25681: <==negation-removal== 77434 (pos)
                    (when (and (at_a_l1))
                          (not (Ba_not_secret_d)))

                    ; #35373: <==uncertain_firing== 76029 (pos)
                    (when (and (not (not_at_d_l1)))
                          (not (Pd_not_secret_d)))

                    ; #44889: <==uncertain_firing== 49982 (pos)
                    (when (and (not (not_at_b_l1)))
                          (not (Bb_not_secret_d)))

                    ; #45173: <==uncertain_firing== 74565 (pos)
                    (when (and (not (not_at_a_l1)))
                          (not (Pa_not_secret_d)))

                    ; #47318: <==negation-removal== 74565 (pos)
                    (when (and (at_a_l1))
                          (not (Pa_not_secret_d)))

                    ; #47557: <==uncertain_firing== 85879 (pos)
                    (when (and (not (not_at_d_l1)))
                          (not (Bd_not_secret_d)))

                    ; #47734: <==uncertain_firing== 36751 (pos)
                    (when (and (not (not_at_c_l1)))
                          (not (Pc_not_secret_d)))

                    ; #52158: <==uncertain_firing== 77434 (pos)
                    (when (and (not (not_at_a_l1)))
                          (not (Ba_not_secret_d)))

                    ; #56215: <==negation-removal== 76029 (pos)
                    (when (and (at_d_l1))
                          (not (Pd_not_secret_d)))

                    ; #57268: <==uncertain_firing== 65285 (pos)
                    (when (and (not (not_at_c_l1)))
                          (not (Bc_not_secret_d)))

                    ; #59894: <==negation-removal== 36751 (pos)
                    (when (and (at_c_l1))
                          (not (Pc_not_secret_d)))

                    ; #70106: <==negation-removal== 85879 (pos)
                    (when (and (at_d_l1))
                          (not (Bd_not_secret_d)))

                    ; #73836: <==uncertain_firing== 40173 (pos)
                    (when (and (not (not_at_b_l1)))
                          (not (Pb_not_secret_d)))

                    ; #77799: <==negation-removal== 40173 (pos)
                    (when (and (at_b_l1))
                          (not (Pb_not_secret_d)))

                    ; #86338: <==negation-removal== 49982 (pos)
                    (when (and (at_b_l1))
                          (not (Bb_not_secret_d)))))

    (:action share_c_d_l2
        :precondition (and (at_c_l2)
                           (Bc_secret_d))
        :effect (and
                    ; #11249: <==closure== 35073 (pos)
                    (when (and (at_a_l2))
                          (Pa_secret_d))

                    ; #19915: origin
                    (when (and (at_b_l2))
                          (Bb_secret_d))

                    ; #35073: origin
                    (when (and (at_a_l2))
                          (Ba_secret_d))

                    ; #44610: <==closure== 72416 (pos)
                    (when (and (at_c_l2))
                          (Pc_secret_d))

                    ; #53454: <==closure== 69245 (pos)
                    (when (and (at_d_l2))
                          (Pd_secret_d))

                    ; #69245: origin
                    (when (and (at_d_l2))
                          (Bd_secret_d))

                    ; #72416: origin
                    (when (and (at_c_l2))
                          (Bc_secret_d))

                    ; #79557: <==closure== 19915 (pos)
                    (when (and (at_b_l2))
                          (Pb_secret_d))

                    ; #14221: <==negation-removal== 11249 (pos)
                    (when (and (at_a_l2))
                          (not (Ba_not_secret_d)))

                    ; #14339: <==negation-removal== 19915 (pos)
                    (when (and (at_b_l2))
                          (not (Pb_not_secret_d)))

                    ; #22461: <==uncertain_firing== 69245 (pos)
                    (when (and (not (not_at_d_l2)))
                          (not (Pd_not_secret_d)))

                    ; #23757: <==negation-removal== 53454 (pos)
                    (when (and (at_d_l2))
                          (not (Bd_not_secret_d)))

                    ; #28915: <==negation-removal== 69245 (pos)
                    (when (and (at_d_l2))
                          (not (Pd_not_secret_d)))

                    ; #31652: <==uncertain_firing== 44610 (pos)
                    (when (and (not (not_at_c_l2)))
                          (not (Bc_not_secret_d)))

                    ; #36397: <==uncertain_firing== 53454 (pos)
                    (when (and (not (not_at_d_l2)))
                          (not (Bd_not_secret_d)))

                    ; #38489: <==negation-removal== 72416 (pos)
                    (when (and (at_c_l2))
                          (not (Pc_not_secret_d)))

                    ; #40408: <==uncertain_firing== 11249 (pos)
                    (when (and (not (not_at_a_l2)))
                          (not (Ba_not_secret_d)))

                    ; #44794: <==uncertain_firing== 19915 (pos)
                    (when (and (not (not_at_b_l2)))
                          (not (Pb_not_secret_d)))

                    ; #55088: <==negation-removal== 35073 (pos)
                    (when (and (at_a_l2))
                          (not (Pa_not_secret_d)))

                    ; #55680: <==uncertain_firing== 72416 (pos)
                    (when (and (not (not_at_c_l2)))
                          (not (Pc_not_secret_d)))

                    ; #61057: <==uncertain_firing== 35073 (pos)
                    (when (and (not (not_at_a_l2)))
                          (not (Pa_not_secret_d)))

                    ; #67298: <==negation-removal== 44610 (pos)
                    (when (and (at_c_l2))
                          (not (Bc_not_secret_d)))

                    ; #76347: <==negation-removal== 79557 (pos)
                    (when (and (at_b_l2))
                          (not (Bb_not_secret_d)))

                    ; #87686: <==uncertain_firing== 79557 (pos)
                    (when (and (not (not_at_b_l2)))
                          (not (Bb_not_secret_d)))))

    (:action share_d_a_l1
        :precondition (and (at_d_l1)
                           (Bd_secret_a))
        :effect (and
                    ; #36751: origin
                    (when (and (at_c_l1))
                          (Bc_secret_a))

                    ; #40173: origin
                    (when (and (at_b_l1))
                          (Bb_secret_a))

                    ; #49982: <==closure== 40173 (pos)
                    (when (and (at_b_l1))
                          (Pb_secret_a))

                    ; #65285: <==closure== 36751 (pos)
                    (when (and (at_c_l1))
                          (Pc_secret_a))

                    ; #74565: origin
                    (when (and (at_a_l1))
                          (Ba_secret_a))

                    ; #76029: origin
                    (when (and (at_d_l1))
                          (Bd_secret_a))

                    ; #77434: <==closure== 74565 (pos)
                    (when (and (at_a_l1))
                          (Pa_secret_a))

                    ; #85879: <==closure== 76029 (pos)
                    (when (and (at_d_l1))
                          (Pd_secret_a))

                    ; #24085: <==negation-removal== 65285 (pos)
                    (when (and (at_c_l1))
                          (not (Bc_not_secret_a)))

                    ; #25681: <==negation-removal== 77434 (pos)
                    (when (and (at_a_l1))
                          (not (Ba_not_secret_a)))

                    ; #35373: <==uncertain_firing== 76029 (pos)
                    (when (and (not (not_at_d_l1)))
                          (not (Pd_not_secret_a)))

                    ; #44889: <==uncertain_firing== 49982 (pos)
                    (when (and (not (not_at_b_l1)))
                          (not (Bb_not_secret_a)))

                    ; #45173: <==uncertain_firing== 74565 (pos)
                    (when (and (not (not_at_a_l1)))
                          (not (Pa_not_secret_a)))

                    ; #47318: <==negation-removal== 74565 (pos)
                    (when (and (at_a_l1))
                          (not (Pa_not_secret_a)))

                    ; #47557: <==uncertain_firing== 85879 (pos)
                    (when (and (not (not_at_d_l1)))
                          (not (Bd_not_secret_a)))

                    ; #47734: <==uncertain_firing== 36751 (pos)
                    (when (and (not (not_at_c_l1)))
                          (not (Pc_not_secret_a)))

                    ; #52158: <==uncertain_firing== 77434 (pos)
                    (when (and (not (not_at_a_l1)))
                          (not (Ba_not_secret_a)))

                    ; #56215: <==negation-removal== 76029 (pos)
                    (when (and (at_d_l1))
                          (not (Pd_not_secret_a)))

                    ; #57268: <==uncertain_firing== 65285 (pos)
                    (when (and (not (not_at_c_l1)))
                          (not (Bc_not_secret_a)))

                    ; #59894: <==negation-removal== 36751 (pos)
                    (when (and (at_c_l1))
                          (not (Pc_not_secret_a)))

                    ; #70106: <==negation-removal== 85879 (pos)
                    (when (and (at_d_l1))
                          (not (Bd_not_secret_a)))

                    ; #73836: <==uncertain_firing== 40173 (pos)
                    (when (and (not (not_at_b_l1)))
                          (not (Pb_not_secret_a)))

                    ; #77799: <==negation-removal== 40173 (pos)
                    (when (and (at_b_l1))
                          (not (Pb_not_secret_a)))

                    ; #86338: <==negation-removal== 49982 (pos)
                    (when (and (at_b_l1))
                          (not (Bb_not_secret_a)))))

    (:action share_d_a_l2
        :precondition (and (at_d_l2)
                           (Bd_secret_a))
        :effect (and
                    ; #11249: <==closure== 35073 (pos)
                    (when (and (at_a_l2))
                          (Pa_secret_a))

                    ; #19915: origin
                    (when (and (at_b_l2))
                          (Bb_secret_a))

                    ; #35073: origin
                    (when (and (at_a_l2))
                          (Ba_secret_a))

                    ; #44610: <==closure== 72416 (pos)
                    (when (and (at_c_l2))
                          (Pc_secret_a))

                    ; #53454: <==closure== 69245 (pos)
                    (when (and (at_d_l2))
                          (Pd_secret_a))

                    ; #69245: origin
                    (when (and (at_d_l2))
                          (Bd_secret_a))

                    ; #72416: origin
                    (when (and (at_c_l2))
                          (Bc_secret_a))

                    ; #79557: <==closure== 19915 (pos)
                    (when (and (at_b_l2))
                          (Pb_secret_a))

                    ; #14221: <==negation-removal== 11249 (pos)
                    (when (and (at_a_l2))
                          (not (Ba_not_secret_a)))

                    ; #14339: <==negation-removal== 19915 (pos)
                    (when (and (at_b_l2))
                          (not (Pb_not_secret_a)))

                    ; #22461: <==uncertain_firing== 69245 (pos)
                    (when (and (not (not_at_d_l2)))
                          (not (Pd_not_secret_a)))

                    ; #23757: <==negation-removal== 53454 (pos)
                    (when (and (at_d_l2))
                          (not (Bd_not_secret_a)))

                    ; #28915: <==negation-removal== 69245 (pos)
                    (when (and (at_d_l2))
                          (not (Pd_not_secret_a)))

                    ; #31652: <==uncertain_firing== 44610 (pos)
                    (when (and (not (not_at_c_l2)))
                          (not (Bc_not_secret_a)))

                    ; #36397: <==uncertain_firing== 53454 (pos)
                    (when (and (not (not_at_d_l2)))
                          (not (Bd_not_secret_a)))

                    ; #38489: <==negation-removal== 72416 (pos)
                    (when (and (at_c_l2))
                          (not (Pc_not_secret_a)))

                    ; #40408: <==uncertain_firing== 11249 (pos)
                    (when (and (not (not_at_a_l2)))
                          (not (Ba_not_secret_a)))

                    ; #44794: <==uncertain_firing== 19915 (pos)
                    (when (and (not (not_at_b_l2)))
                          (not (Pb_not_secret_a)))

                    ; #55088: <==negation-removal== 35073 (pos)
                    (when (and (at_a_l2))
                          (not (Pa_not_secret_a)))

                    ; #55680: <==uncertain_firing== 72416 (pos)
                    (when (and (not (not_at_c_l2)))
                          (not (Pc_not_secret_a)))

                    ; #61057: <==uncertain_firing== 35073 (pos)
                    (when (and (not (not_at_a_l2)))
                          (not (Pa_not_secret_a)))

                    ; #67298: <==negation-removal== 44610 (pos)
                    (when (and (at_c_l2))
                          (not (Bc_not_secret_a)))

                    ; #76347: <==negation-removal== 79557 (pos)
                    (when (and (at_b_l2))
                          (not (Bb_not_secret_a)))

                    ; #87686: <==uncertain_firing== 79557 (pos)
                    (when (and (not (not_at_b_l2)))
                          (not (Bb_not_secret_a)))))

    (:action share_d_b_l1
        :precondition (and (at_d_l1)
                           (Bd_secret_b))
        :effect (and
                    ; #36751: origin
                    (when (and (at_c_l1))
                          (Bc_secret_b))

                    ; #40173: origin
                    (when (and (at_b_l1))
                          (Bb_secret_b))

                    ; #49982: <==closure== 40173 (pos)
                    (when (and (at_b_l1))
                          (Pb_secret_b))

                    ; #65285: <==closure== 36751 (pos)
                    (when (and (at_c_l1))
                          (Pc_secret_b))

                    ; #74565: origin
                    (when (and (at_a_l1))
                          (Ba_secret_b))

                    ; #76029: origin
                    (when (and (at_d_l1))
                          (Bd_secret_b))

                    ; #77434: <==closure== 74565 (pos)
                    (when (and (at_a_l1))
                          (Pa_secret_b))

                    ; #85879: <==closure== 76029 (pos)
                    (when (and (at_d_l1))
                          (Pd_secret_b))

                    ; #24085: <==negation-removal== 65285 (pos)
                    (when (and (at_c_l1))
                          (not (Bc_not_secret_b)))

                    ; #25681: <==negation-removal== 77434 (pos)
                    (when (and (at_a_l1))
                          (not (Ba_not_secret_b)))

                    ; #35373: <==uncertain_firing== 76029 (pos)
                    (when (and (not (not_at_d_l1)))
                          (not (Pd_not_secret_b)))

                    ; #44889: <==uncertain_firing== 49982 (pos)
                    (when (and (not (not_at_b_l1)))
                          (not (Bb_not_secret_b)))

                    ; #45173: <==uncertain_firing== 74565 (pos)
                    (when (and (not (not_at_a_l1)))
                          (not (Pa_not_secret_b)))

                    ; #47318: <==negation-removal== 74565 (pos)
                    (when (and (at_a_l1))
                          (not (Pa_not_secret_b)))

                    ; #47557: <==uncertain_firing== 85879 (pos)
                    (when (and (not (not_at_d_l1)))
                          (not (Bd_not_secret_b)))

                    ; #47734: <==uncertain_firing== 36751 (pos)
                    (when (and (not (not_at_c_l1)))
                          (not (Pc_not_secret_b)))

                    ; #52158: <==uncertain_firing== 77434 (pos)
                    (when (and (not (not_at_a_l1)))
                          (not (Ba_not_secret_b)))

                    ; #56215: <==negation-removal== 76029 (pos)
                    (when (and (at_d_l1))
                          (not (Pd_not_secret_b)))

                    ; #57268: <==uncertain_firing== 65285 (pos)
                    (when (and (not (not_at_c_l1)))
                          (not (Bc_not_secret_b)))

                    ; #59894: <==negation-removal== 36751 (pos)
                    (when (and (at_c_l1))
                          (not (Pc_not_secret_b)))

                    ; #70106: <==negation-removal== 85879 (pos)
                    (when (and (at_d_l1))
                          (not (Bd_not_secret_b)))

                    ; #73836: <==uncertain_firing== 40173 (pos)
                    (when (and (not (not_at_b_l1)))
                          (not (Pb_not_secret_b)))

                    ; #77799: <==negation-removal== 40173 (pos)
                    (when (and (at_b_l1))
                          (not (Pb_not_secret_b)))

                    ; #86338: <==negation-removal== 49982 (pos)
                    (when (and (at_b_l1))
                          (not (Bb_not_secret_b)))))

    (:action share_d_b_l2
        :precondition (and (at_d_l2)
                           (Bd_secret_b))
        :effect (and
                    ; #11249: <==closure== 35073 (pos)
                    (when (and (at_a_l2))
                          (Pa_secret_b))

                    ; #19915: origin
                    (when (and (at_b_l2))
                          (Bb_secret_b))

                    ; #35073: origin
                    (when (and (at_a_l2))
                          (Ba_secret_b))

                    ; #44610: <==closure== 72416 (pos)
                    (when (and (at_c_l2))
                          (Pc_secret_b))

                    ; #53454: <==closure== 69245 (pos)
                    (when (and (at_d_l2))
                          (Pd_secret_b))

                    ; #69245: origin
                    (when (and (at_d_l2))
                          (Bd_secret_b))

                    ; #72416: origin
                    (when (and (at_c_l2))
                          (Bc_secret_b))

                    ; #79557: <==closure== 19915 (pos)
                    (when (and (at_b_l2))
                          (Pb_secret_b))

                    ; #14221: <==negation-removal== 11249 (pos)
                    (when (and (at_a_l2))
                          (not (Ba_not_secret_b)))

                    ; #14339: <==negation-removal== 19915 (pos)
                    (when (and (at_b_l2))
                          (not (Pb_not_secret_b)))

                    ; #22461: <==uncertain_firing== 69245 (pos)
                    (when (and (not (not_at_d_l2)))
                          (not (Pd_not_secret_b)))

                    ; #23757: <==negation-removal== 53454 (pos)
                    (when (and (at_d_l2))
                          (not (Bd_not_secret_b)))

                    ; #28915: <==negation-removal== 69245 (pos)
                    (when (and (at_d_l2))
                          (not (Pd_not_secret_b)))

                    ; #31652: <==uncertain_firing== 44610 (pos)
                    (when (and (not (not_at_c_l2)))
                          (not (Bc_not_secret_b)))

                    ; #36397: <==uncertain_firing== 53454 (pos)
                    (when (and (not (not_at_d_l2)))
                          (not (Bd_not_secret_b)))

                    ; #38489: <==negation-removal== 72416 (pos)
                    (when (and (at_c_l2))
                          (not (Pc_not_secret_b)))

                    ; #40408: <==uncertain_firing== 11249 (pos)
                    (when (and (not (not_at_a_l2)))
                          (not (Ba_not_secret_b)))

                    ; #44794: <==uncertain_firing== 19915 (pos)
                    (when (and (not (not_at_b_l2)))
                          (not (Pb_not_secret_b)))

                    ; #55088: <==negation-removal== 35073 (pos)
                    (when (and (at_a_l2))
                          (not (Pa_not_secret_b)))

                    ; #55680: <==uncertain_firing== 72416 (pos)
                    (when (and (not (not_at_c_l2)))
                          (not (Pc_not_secret_b)))

                    ; #61057: <==uncertain_firing== 35073 (pos)
                    (when (and (not (not_at_a_l2)))
                          (not (Pa_not_secret_b)))

                    ; #67298: <==negation-removal== 44610 (pos)
                    (when (and (at_c_l2))
                          (not (Bc_not_secret_b)))

                    ; #76347: <==negation-removal== 79557 (pos)
                    (when (and (at_b_l2))
                          (not (Bb_not_secret_b)))

                    ; #87686: <==uncertain_firing== 79557 (pos)
                    (when (and (not (not_at_b_l2)))
                          (not (Bb_not_secret_b)))))

    (:action share_d_c_l1
        :precondition (and (at_d_l1)
                           (Bd_secret_c))
        :effect (and
                    ; #36751: origin
                    (when (and (at_c_l1))
                          (Bc_secret_c))

                    ; #40173: origin
                    (when (and (at_b_l1))
                          (Bb_secret_c))

                    ; #49982: <==closure== 40173 (pos)
                    (when (and (at_b_l1))
                          (Pb_secret_c))

                    ; #65285: <==closure== 36751 (pos)
                    (when (and (at_c_l1))
                          (Pc_secret_c))

                    ; #74565: origin
                    (when (and (at_a_l1))
                          (Ba_secret_c))

                    ; #76029: origin
                    (when (and (at_d_l1))
                          (Bd_secret_c))

                    ; #77434: <==closure== 74565 (pos)
                    (when (and (at_a_l1))
                          (Pa_secret_c))

                    ; #85879: <==closure== 76029 (pos)
                    (when (and (at_d_l1))
                          (Pd_secret_c))

                    ; #24085: <==negation-removal== 65285 (pos)
                    (when (and (at_c_l1))
                          (not (Bc_not_secret_c)))

                    ; #25681: <==negation-removal== 77434 (pos)
                    (when (and (at_a_l1))
                          (not (Ba_not_secret_c)))

                    ; #35373: <==uncertain_firing== 76029 (pos)
                    (when (and (not (not_at_d_l1)))
                          (not (Pd_not_secret_c)))

                    ; #44889: <==uncertain_firing== 49982 (pos)
                    (when (and (not (not_at_b_l1)))
                          (not (Bb_not_secret_c)))

                    ; #45173: <==uncertain_firing== 74565 (pos)
                    (when (and (not (not_at_a_l1)))
                          (not (Pa_not_secret_c)))

                    ; #47318: <==negation-removal== 74565 (pos)
                    (when (and (at_a_l1))
                          (not (Pa_not_secret_c)))

                    ; #47557: <==uncertain_firing== 85879 (pos)
                    (when (and (not (not_at_d_l1)))
                          (not (Bd_not_secret_c)))

                    ; #47734: <==uncertain_firing== 36751 (pos)
                    (when (and (not (not_at_c_l1)))
                          (not (Pc_not_secret_c)))

                    ; #52158: <==uncertain_firing== 77434 (pos)
                    (when (and (not (not_at_a_l1)))
                          (not (Ba_not_secret_c)))

                    ; #56215: <==negation-removal== 76029 (pos)
                    (when (and (at_d_l1))
                          (not (Pd_not_secret_c)))

                    ; #57268: <==uncertain_firing== 65285 (pos)
                    (when (and (not (not_at_c_l1)))
                          (not (Bc_not_secret_c)))

                    ; #59894: <==negation-removal== 36751 (pos)
                    (when (and (at_c_l1))
                          (not (Pc_not_secret_c)))

                    ; #70106: <==negation-removal== 85879 (pos)
                    (when (and (at_d_l1))
                          (not (Bd_not_secret_c)))

                    ; #73836: <==uncertain_firing== 40173 (pos)
                    (when (and (not (not_at_b_l1)))
                          (not (Pb_not_secret_c)))

                    ; #77799: <==negation-removal== 40173 (pos)
                    (when (and (at_b_l1))
                          (not (Pb_not_secret_c)))

                    ; #86338: <==negation-removal== 49982 (pos)
                    (when (and (at_b_l1))
                          (not (Bb_not_secret_c)))))

    (:action share_d_c_l2
        :precondition (and (at_d_l2)
                           (Bd_secret_c))
        :effect (and
                    ; #11249: <==closure== 35073 (pos)
                    (when (and (at_a_l2))
                          (Pa_secret_c))

                    ; #19915: origin
                    (when (and (at_b_l2))
                          (Bb_secret_c))

                    ; #35073: origin
                    (when (and (at_a_l2))
                          (Ba_secret_c))

                    ; #44610: <==closure== 72416 (pos)
                    (when (and (at_c_l2))
                          (Pc_secret_c))

                    ; #53454: <==closure== 69245 (pos)
                    (when (and (at_d_l2))
                          (Pd_secret_c))

                    ; #69245: origin
                    (when (and (at_d_l2))
                          (Bd_secret_c))

                    ; #72416: origin
                    (when (and (at_c_l2))
                          (Bc_secret_c))

                    ; #79557: <==closure== 19915 (pos)
                    (when (and (at_b_l2))
                          (Pb_secret_c))

                    ; #14221: <==negation-removal== 11249 (pos)
                    (when (and (at_a_l2))
                          (not (Ba_not_secret_c)))

                    ; #14339: <==negation-removal== 19915 (pos)
                    (when (and (at_b_l2))
                          (not (Pb_not_secret_c)))

                    ; #22461: <==uncertain_firing== 69245 (pos)
                    (when (and (not (not_at_d_l2)))
                          (not (Pd_not_secret_c)))

                    ; #23757: <==negation-removal== 53454 (pos)
                    (when (and (at_d_l2))
                          (not (Bd_not_secret_c)))

                    ; #28915: <==negation-removal== 69245 (pos)
                    (when (and (at_d_l2))
                          (not (Pd_not_secret_c)))

                    ; #31652: <==uncertain_firing== 44610 (pos)
                    (when (and (not (not_at_c_l2)))
                          (not (Bc_not_secret_c)))

                    ; #36397: <==uncertain_firing== 53454 (pos)
                    (when (and (not (not_at_d_l2)))
                          (not (Bd_not_secret_c)))

                    ; #38489: <==negation-removal== 72416 (pos)
                    (when (and (at_c_l2))
                          (not (Pc_not_secret_c)))

                    ; #40408: <==uncertain_firing== 11249 (pos)
                    (when (and (not (not_at_a_l2)))
                          (not (Ba_not_secret_c)))

                    ; #44794: <==uncertain_firing== 19915 (pos)
                    (when (and (not (not_at_b_l2)))
                          (not (Pb_not_secret_c)))

                    ; #55088: <==negation-removal== 35073 (pos)
                    (when (and (at_a_l2))
                          (not (Pa_not_secret_c)))

                    ; #55680: <==uncertain_firing== 72416 (pos)
                    (when (and (not (not_at_c_l2)))
                          (not (Pc_not_secret_c)))

                    ; #61057: <==uncertain_firing== 35073 (pos)
                    (when (and (not (not_at_a_l2)))
                          (not (Pa_not_secret_c)))

                    ; #67298: <==negation-removal== 44610 (pos)
                    (when (and (at_c_l2))
                          (not (Bc_not_secret_c)))

                    ; #76347: <==negation-removal== 79557 (pos)
                    (when (and (at_b_l2))
                          (not (Bb_not_secret_c)))

                    ; #87686: <==uncertain_firing== 79557 (pos)
                    (when (and (not (not_at_b_l2)))
                          (not (Bb_not_secret_c)))))

    (:action share_d_d_l1
        :precondition (and (Bd_secret_d)
                           (at_d_l1))
        :effect (and
                    ; #36751: origin
                    (when (and (at_c_l1))
                          (Bc_secret_d))

                    ; #40173: origin
                    (when (and (at_b_l1))
                          (Bb_secret_d))

                    ; #49982: <==closure== 40173 (pos)
                    (when (and (at_b_l1))
                          (Pb_secret_d))

                    ; #65285: <==closure== 36751 (pos)
                    (when (and (at_c_l1))
                          (Pc_secret_d))

                    ; #74565: origin
                    (when (and (at_a_l1))
                          (Ba_secret_d))

                    ; #76029: origin
                    (when (and (at_d_l1))
                          (Bd_secret_d))

                    ; #77434: <==closure== 74565 (pos)
                    (when (and (at_a_l1))
                          (Pa_secret_d))

                    ; #85879: <==closure== 76029 (pos)
                    (when (and (at_d_l1))
                          (Pd_secret_d))

                    ; #24085: <==negation-removal== 65285 (pos)
                    (when (and (at_c_l1))
                          (not (Bc_not_secret_d)))

                    ; #25681: <==negation-removal== 77434 (pos)
                    (when (and (at_a_l1))
                          (not (Ba_not_secret_d)))

                    ; #35373: <==uncertain_firing== 76029 (pos)
                    (when (and (not (not_at_d_l1)))
                          (not (Pd_not_secret_d)))

                    ; #44889: <==uncertain_firing== 49982 (pos)
                    (when (and (not (not_at_b_l1)))
                          (not (Bb_not_secret_d)))

                    ; #45173: <==uncertain_firing== 74565 (pos)
                    (when (and (not (not_at_a_l1)))
                          (not (Pa_not_secret_d)))

                    ; #47318: <==negation-removal== 74565 (pos)
                    (when (and (at_a_l1))
                          (not (Pa_not_secret_d)))

                    ; #47557: <==uncertain_firing== 85879 (pos)
                    (when (and (not (not_at_d_l1)))
                          (not (Bd_not_secret_d)))

                    ; #47734: <==uncertain_firing== 36751 (pos)
                    (when (and (not (not_at_c_l1)))
                          (not (Pc_not_secret_d)))

                    ; #52158: <==uncertain_firing== 77434 (pos)
                    (when (and (not (not_at_a_l1)))
                          (not (Ba_not_secret_d)))

                    ; #56215: <==negation-removal== 76029 (pos)
                    (when (and (at_d_l1))
                          (not (Pd_not_secret_d)))

                    ; #57268: <==uncertain_firing== 65285 (pos)
                    (when (and (not (not_at_c_l1)))
                          (not (Bc_not_secret_d)))

                    ; #59894: <==negation-removal== 36751 (pos)
                    (when (and (at_c_l1))
                          (not (Pc_not_secret_d)))

                    ; #70106: <==negation-removal== 85879 (pos)
                    (when (and (at_d_l1))
                          (not (Bd_not_secret_d)))

                    ; #73836: <==uncertain_firing== 40173 (pos)
                    (when (and (not (not_at_b_l1)))
                          (not (Pb_not_secret_d)))

                    ; #77799: <==negation-removal== 40173 (pos)
                    (when (and (at_b_l1))
                          (not (Pb_not_secret_d)))

                    ; #86338: <==negation-removal== 49982 (pos)
                    (when (and (at_b_l1))
                          (not (Bb_not_secret_d)))))

    (:action share_d_d_l2
        :precondition (and (at_d_l2)
                           (Bd_secret_d))
        :effect (and
                    ; #11249: <==closure== 35073 (pos)
                    (when (and (at_a_l2))
                          (Pa_secret_d))

                    ; #19915: origin
                    (when (and (at_b_l2))
                          (Bb_secret_d))

                    ; #35073: origin
                    (when (and (at_a_l2))
                          (Ba_secret_d))

                    ; #44610: <==closure== 72416 (pos)
                    (when (and (at_c_l2))
                          (Pc_secret_d))

                    ; #53454: <==closure== 69245 (pos)
                    (when (and (at_d_l2))
                          (Pd_secret_d))

                    ; #69245: origin
                    (when (and (at_d_l2))
                          (Bd_secret_d))

                    ; #72416: origin
                    (when (and (at_c_l2))
                          (Bc_secret_d))

                    ; #79557: <==closure== 19915 (pos)
                    (when (and (at_b_l2))
                          (Pb_secret_d))

                    ; #14221: <==negation-removal== 11249 (pos)
                    (when (and (at_a_l2))
                          (not (Ba_not_secret_d)))

                    ; #14339: <==negation-removal== 19915 (pos)
                    (when (and (at_b_l2))
                          (not (Pb_not_secret_d)))

                    ; #22461: <==uncertain_firing== 69245 (pos)
                    (when (and (not (not_at_d_l2)))
                          (not (Pd_not_secret_d)))

                    ; #23757: <==negation-removal== 53454 (pos)
                    (when (and (at_d_l2))
                          (not (Bd_not_secret_d)))

                    ; #28915: <==negation-removal== 69245 (pos)
                    (when (and (at_d_l2))
                          (not (Pd_not_secret_d)))

                    ; #31652: <==uncertain_firing== 44610 (pos)
                    (when (and (not (not_at_c_l2)))
                          (not (Bc_not_secret_d)))

                    ; #36397: <==uncertain_firing== 53454 (pos)
                    (when (and (not (not_at_d_l2)))
                          (not (Bd_not_secret_d)))

                    ; #38489: <==negation-removal== 72416 (pos)
                    (when (and (at_c_l2))
                          (not (Pc_not_secret_d)))

                    ; #40408: <==uncertain_firing== 11249 (pos)
                    (when (and (not (not_at_a_l2)))
                          (not (Ba_not_secret_d)))

                    ; #44794: <==uncertain_firing== 19915 (pos)
                    (when (and (not (not_at_b_l2)))
                          (not (Pb_not_secret_d)))

                    ; #55088: <==negation-removal== 35073 (pos)
                    (when (and (at_a_l2))
                          (not (Pa_not_secret_d)))

                    ; #55680: <==uncertain_firing== 72416 (pos)
                    (when (and (not (not_at_c_l2)))
                          (not (Pc_not_secret_d)))

                    ; #61057: <==uncertain_firing== 35073 (pos)
                    (when (and (not (not_at_a_l2)))
                          (not (Pa_not_secret_d)))

                    ; #67298: <==negation-removal== 44610 (pos)
                    (when (and (at_c_l2))
                          (not (Bc_not_secret_d)))

                    ; #76347: <==negation-removal== 79557 (pos)
                    (when (and (at_b_l2))
                          (not (Bb_not_secret_d)))

                    ; #87686: <==uncertain_firing== 79557 (pos)
                    (when (and (not (not_at_b_l2)))
                          (not (Bb_not_secret_d)))))

)