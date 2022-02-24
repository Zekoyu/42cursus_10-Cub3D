/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   frame_counter.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mframbou <mframbou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/07 14:34:14 by mframbou          #+#    #+#             */
/*   Updated: 2021/12/07 15:22:08 by mframbou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

static long	*get_current_frame_ptr(void)
{
	static long	current_frame = 1;

	return (&current_frame);
}

void	add_curent_frame(void)
{
	long	*curr;

	curr = get_current_frame_ptr();
	*curr += 1;
}

long	get_current_frame(void)
{
	return (*get_current_frame_ptr());
}
